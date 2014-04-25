require 'mkmf'


$CFLAGS = "-Wall -g -O2"


dir_config('mtp')

dir_config('usb')


if(have_header("libmtp.h"))

  if(have_header("usb.h"))

    if(have_library("mtp", "LIBMTP_Get_First_Device"))

      puts "Creating makefile\n\n"

      create_makefile("device/LibMTPBase")

    else

      puts "ERROR - Cannot find libmtp library\n\n"

    end

  else

    puts "ERROR - Cannot find usb.h\n\n"

  end

else

  puts "ERROR - Cannot find libmtp.h\n\n"

end
