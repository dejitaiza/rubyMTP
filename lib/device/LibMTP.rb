##########################################################################
#
# = LibMTP
#
# libMTP ruby extension
#
# Copyright (c) 2007 Todd Olivas (todd@topstorm.org)
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2 of the License, or (at your
# option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
# License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# See LibMTP for detailed documentation.
#
# Download: http://rubyforge.org/projects/libmtp/
#
# History:
# v0.0.1        alpha code      Wed Jan 24 23:23:00 EST 2007
#
########################################################################

require 'device/LibMTPBase'

module LibMTP

#
#  call-seq:
#     LibMTP::connect() -> Connected LibMTP::Device object.
#  
#  Returns a connected LibMTP::Device object unless it is called with a block.
#  In this case the block will be called, and a connected LibMTP::Device object
#  will be passed into the block.  When the block ends, the device will be
#  disconnected.

  def self.connect()
    device = LibMTP::Device.new

    if(block_given?)
      yield(device)

      device = nil
    end

    return device
  end


#
#  call-seq:
#     LibMTP::connect_each() -> Connected LibMTP::Device object.
#  
#  Returns an array of connected LibMTP::Device objects unless it is called with a block.
#  In this case the block will be called once for each device.

  def self.connect_each()
    array = LibMTP::Device.list()

    if(block_given?)
      array.each do |device|
        yield(device)
      end

      array = nil
    end

    return array
  end

end
