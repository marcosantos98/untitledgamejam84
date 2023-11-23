
print(RED.r)

function render()

	if IsKeyDown(KEY_D) then
		data.playerX = data.playerX + 1
	elseif IsKeyDown(KEY_A) then
		data.playerX = data.playerX - 1
	end

	if IsKeyDown(KEY_W) then
		data.playerY = data.playerY - 1
	elseif IsKeyDown(KEY_S) then
		data.playerY = data.playerY + 1
	end



	DrawRect(data.playerX, data.playerY, 100, 100, RED);	
end

