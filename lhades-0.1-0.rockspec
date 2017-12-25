package = "lhades"
version = "0.1-0"

source = {
	url = "git://github.com/tacigar/lhades.git",
}

description = {
	summary= "Lua 5.3 Byte-Code Disassembler",
	detailed= [[
		Disassemble byte codes of Lua 5.3, and output that info.
	]],
	license = "GPLv3",
	homepage = "https://github.com/tacigar/lhades",
	maintainer = "tacigar",
}

dependencies = {
	"lua >= 5.3",
}

build = {
	type = "make",
	build_variables = {
		LUA_INCDIR = "$(LUA_INCDIR)",
	},
	install_variables = {
		INST_BINDIR = "$(BINDIR)",
	},
}
