--[[
	Very rough dds reader that assumes:
	 - it is a dds file
	 - it is 16bpp
	 - it is uncompressed
]]

print("Converting font...")

local font = io.open("raw.dds")
font:seek("cur", 128)

local src = io.open("../src/display/font.h", "w")
src:write("static const char g_font[]={")

local byte = 0
local first = true
for y = 1, 8 do
	for x = 1, 2048 do
		byte = byte * 2
		if font:read(2):byte() > 0 then
			byte = byte + 1
		end

		if x % 8 == 0 then
			if first then
				first = false
			else
				src:write(",")
			end
			src:write(byte)
			byte = 0
		end
	end
end

src:write("};")
src:close()
font:close()