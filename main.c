#include <stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <raylib.h>

#define SAFE_LUA(L, name) 						\
	do { 										\
		if(luaL_dofile(L, name) != LUA_OK) { 	\
			printf("\033[31mLua error:\033[39m %s\n", lua_tostring(L, -1)); \
			goto end; 						\
		} 									\
	} while(0); 								\

#define PUSH_COLOR(L, color) push_raylib_color(L, color, #color)
#define PUSH_KEY(L, key) push_raylib_key(L, key, #key)

typedef struct {
	int playerX;
	int playerY;
} GameData;

static GameData data = {0};

static void dumpstack (lua_State *L) {
  int top=lua_gettop(L);
  for (int i=1; i <= top; i++) {
    printf("%d\t%s\t", i, luaL_typename(L,i));
    switch (lua_type(L, i)) {
      case LUA_TNUMBER:
        printf("%g\n",lua_tonumber(L,i));
        break;
      case LUA_TSTRING:
        printf("%s\n",lua_tostring(L,i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n",lua_topointer(L,i));
        break;
    }
  }
}

int try_pop_number(lua_State* L) {
	if(lua_isnumber(L, -1)) {
		int r = lua_tonumber(L, -1);
		lua_pop(L, 1);
		return r;
	} else {
		printf("Trying to convert to number on a non number value!.\n");
		return 0;
	}
}

void push_raylib_color(lua_State* L, Color c, const char* name) {
	lua_newtable(L);
	lua_pushnumber(L, c.r);
	lua_setfield(L, -2, "r");
	lua_pushnumber(L, c.g);
	lua_setfield(L, -2, "g");
	lua_pushnumber(L, c.b);
	lua_setfield(L, -2, "b");
	lua_pushnumber(L, c.a);
	lua_setfield(L, -2, "a");
	lua_setglobal(L, name);
}

void push_raylib_key(lua_State* L, int key, const char* name) {
	lua_pushnumber(L, key);
	lua_setglobal(L, name);
}

//TODO: Check types!
Color table_to_color(lua_State* L) {
	lua_getfield(L, -1, "r");
	int r = lua_tonumber(L, -1);
	lua_getfield(L, -2, "g");
	int g = lua_tonumber(L, -1);
	lua_getfield(L, -3, "b");
	int b = lua_tonumber(L, -1);
	lua_getfield(L, -4, "a");
	int a = lua_tonumber(L, -1);
	lua_pop(L, 5); // Remove all pushed color values and the color table
	return (Color) {
		r, g, b, a,
	};
}

int call_raylib_drawrectangle(lua_State* L) {
	Color c = table_to_color(L);

	int h = try_pop_number(L);
	int w = try_pop_number(L);
	int y = try_pop_number(L);
	int x = try_pop_number(L);

	DrawRectangle(x, y, w, h, c);

	return 0;
}

int call_raylib_iskeydown(lua_State* L) {
	int key = try_pop_number(L);
	lua_pushboolean(L, IsKeyDown(key));
	return 1;
}

void setup_lua(lua_State* L) {
	luaL_openlibs(L); //Load std libraries from lua.
	
	// Create Data Table
	lua_newtable(L); // data global index -1

	// Stack -1 is the table itself, 
	// so we index with minus two to get the number we pushed
	lua_pushnumber(L, data.playerX);
	lua_setfield(L, -2, "playerX");
	lua_pushnumber(L, data.playerY);
	lua_setfield(L, -2, "playerY");

	lua_setglobal(L, "data");

	PUSH_COLOR(L, RED);

	PUSH_KEY(L, KEY_D);
	PUSH_KEY(L, KEY_A);
	PUSH_KEY(L, KEY_W);
	PUSH_KEY(L, KEY_S);

	lua_register(L, "DrawRect", call_raylib_drawrectangle);
	lua_register(L, "IsKeyDown", call_raylib_iskeydown);

}

int main(void) {

	InitWindow(1280, 720, "untitledgamejam-84 - Mushrooms");

	lua_State* L = luaL_newstate();
	
	setup_lua(L);

	SAFE_LUA(L, "main.lua");

	while(!WindowShouldClose()){
		ClearBackground(RAYWHITE);
		BeginDrawing();
		
		lua_getglobal(L, "render");
		lua_call(L, 0, 0);

		EndDrawing();
	}


end:
	lua_close(L);
	CloseWindow();
	return 0;
}
