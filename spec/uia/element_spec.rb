require 'spec_helper'

describe Uia::Element do
  Given(:element) { Uia.find_element(id: 'MainFormWindow') }
  Given(:about_box) { Uia.find_element(id: 'AboutBox') }

  context 'properties' do
    let(:raw_element) { element.instance_variable_get(:@element) }
    Then { element.handle != 0 }
    Then { element.name == 'MainFormWindow' }
    Then { element.id == 'MainFormWindow' }
    Then { element.class_name =~ /Forms.*app/i }
    Then { expect(element.find(id: 'textField')).to be_enabled }

    context '#control_type' do
      Then { element.control_type == :window }

      context 'unknown' do
        Given { raw_element.stub(:control_type_id).and_return(777) }
        Then { element.control_type == :unknown }
      end
    end

    context '#patterns' do
      Then { element.patterns.should =~ [:transform, :window] }

      context 'unknown' do
        Given { raw_element.stub(:pattern_ids).and_return([7777]) }
        Then { element.patterns.should == [:unknown] }
      end

      context '#as' do
        When(:cast) { element.as :toggle }
        Then { cast.should have_failed UnsupportedPattern, "Pattern toggle not found in [:window, :transform]" }
      end
    end

    context '#refresh' do
      Given!(:check_box) do
        check_box = element.find(id: 'checkBox').as :toggle
        check_box.toggle_state = :off
        check_box
      end

      Given!(:label) { element.find(id: 'checkBoxLabel') }
      When { check_box.toggle_state = :on; label.refresh }
      Then { label.name == 'checkBox is on' }
    end
  end

  context '#find' do
    context 'id' do
      Then { element.find(id: 'textField') != nil }
      Then { element.find(id: 'does not exist') == nil }
    end

    context 'name' do
      Then { element.find(name: 'No option selected') != nil }
      Then { element.find(name: 'does not exist') == nil }
    end

    context 'limiting scope' do
      Then { element.find(name: 'label2', scope: :children) == nil }
    end
  end

  context '#select' do
    context 'control_type' do
      When(:buttons) { element.select(control_type: :radio_button) }
      Then { buttons.map(&:control_type) == [:radio_button] * 3 }
    end

    context 'pattern' do
      Then { element.select(pattern: :value).count == 4 }
    end

    context 'combinations' do
      Then { element.select(control_type: :button, name: 'About')[0].id == 'aboutButton' }
    end
  end

  context '#click' do
    Given(:about) { element.children.find { |c| c.name == 'About' } }
    Given(:disabled_checkbox) { element.children.find { |c| c.name == 'checkBoxDisabled' } }

    context 'enabled elements' do
      When { about.click }
      Then { about_box != nil }
      And { about_box.children.find { |c| c.name == 'OK' }.click }
    end

    context 'disabled elements' do
      When(:click_disabled) { disabled_checkbox.click }
      Then { click_disabled.should have_failed('Target element cannot receive focus.') }
    end
  end

  context '#children' do
    Then { element.children.count == 27 }
    Then { element.children.all? { |c| c.instance_of? Uia::Element } }
  end

  context '#descendants' do
    Then { element.descendants.count > element.children.count }
    Then { element.descendants.all? { |c| c.instance_of? Uia::Element } }
  end
end
