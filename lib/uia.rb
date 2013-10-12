require 'uia/version'
require 'uia/element/elements'
require 'uia/library'
require 'ffi'

module Uia
  def find_element(how)
    case
      when how[:id]
        Library.find_by_id(how[:id])
      when how[:pid]
        Library.find_by_pid(how[:pid])
      when how[:runtime_id]
        Library.find_by_runtime_id(how[:runtime_id])
      when how[:handle]
        Library.find_by_handle(how[:handle])
      else
        raise "#{how} is not a valid locator"
    end
  end
end
