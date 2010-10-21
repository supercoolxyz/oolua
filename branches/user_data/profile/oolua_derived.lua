local N = 10
local ave = 0
local times = 1000000

for i = 0, N do

	local obj = ProfileDerived:new()
	local increment_me = ProfileDerived:new()


	local t0 = os.clock()
	for i=1,times do
		obj:increment_a_base(increment_me)
	end
	local dt = os.clock()-t0
	if i~=0 then
	 ave = ave + dt
	end

end

print('OOLua passing derived to a function that wants a base  (average elapsed time):',ave/N)

ave = 0
for i = 0, N do

	local obj = ProfileDerived:new()
	local increment_me = ProfileMultiBases:new()


	local t0 = os.clock()
	for i=1,times do
		obj:increment_a_base(increment_me)
	end
	local dt = os.clock()-t0
	if i~=0 then
	 ave = ave + dt
	end

end

print('OOLua passing derived with multiple bases to a function that wants a base  (average elapsed time):',ave/N)


ave = 0
for i = 0, N do

	local obj = ProfileMultiBases:new()


	local t0 = os.clock()
	for i=1,times do
		obj:virtual_func()
	end
	local dt = os.clock()-t0
	if i~=0 then
	 ave = ave + dt
	end

end

print('OOLua virtual function  (average elapsed time):',ave/N)



ave = 0
for i = 0, N do

	local obj = ProfileMultiBases:new()


	local t0 = os.clock()
	for i=1,times do
		obj:pure_virtual_func()
	end
	local dt = os.clock()-t0
	if i~=0 then
	 ave = ave + dt
	end

end

print('OOLua pure virtual function  (average elapsed time):',ave/N)

func = function(obj)
ave = 0

	for i = 0, N do
		local t0 = os.clock()
		for i=1,times do
			obj:pure_virtual_func()
		end
		local dt = os.clock()-t0
		if i~=0 then
		 ave = ave + dt
		end
	end
	
	return ave/N
end