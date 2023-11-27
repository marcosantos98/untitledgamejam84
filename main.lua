function tprint (tbl, indent)
  if not indent then indent = 0 end
  local toprint = string.rep(" ", indent) .. "{\r\n"
  indent = indent + 2 
  for k, v in pairs(tbl) do
    toprint = toprint .. string.rep(" ", indent)
    if (type(k) == "number") then
      toprint = toprint .. "[" .. k .. "] = "
    elseif (type(k) == "string") then
      toprint = toprint  .. k ..  "= "   
    end
    if (type(v) == "number") then
      toprint = toprint .. v .. ",\r\n"
    elseif (type(v) == "string") then
      toprint = toprint .. "\"" .. v .. "\",\r\n"
    elseif (type(v) == "table") then
      toprint = toprint .. tprint(v, indent + 2) .. ",\r\n"
    else
      toprint = toprint .. "\"" .. tostring(v) .. "\",\r\n"
    end
  end
  toprint = toprint .. string.rep(" ", indent-2) .. "}"
  return toprint
end

print(tprint(data, 0))

function render()

	if IsKeyDown(KEY_D) then
		data.player.x = data.player.x + 1
	elseif IsKeyDown(KEY_A) then
		data.player.x = data.player.x - 1
	end

	if IsKeyDown(KEY_W) then
		data.player.y = data.player.y - 1
	elseif IsKeyDown(KEY_S) then
		data.player.y = data.player.y + 1
	end

	DrawRect(data.player.x, data.player.y, 100, 100, RED);
	DrawRect(data.player.x - 10, data.player.y - 10, 50, 50, {r = 0, g = 255, b = 0, a = 255});

	if IsMouseButtonPressed(MOUSE_BUTTON_LEFT) then
		print("Pressed Mouse Left Button")
	end
end


