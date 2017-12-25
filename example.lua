local x = 7
local function f(y)
	return function()
		return x + y
	end
end
local y = f(3)

for i = 1, 5 do
	print(y())
end
