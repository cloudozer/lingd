-module(bootling).
-export([start/1]).

start(Node) ->
	store_string("/io/" ++ Node ++ "/run/bootling/ipaddr", eth0_ipaddr()),

	application:start(yamerl),
	lists:foreach(
		fun(Y) ->
			Domain = filename:rootname(filename:basename(Y)),
			[Yml] = yamerl_constr:file(Y),
			App = proplists:get_value("app", Yml),
			Dir = "/io/app/" ++ App,
			Img = Dir ++ "/" ++ App ++ ".img",

			ScriptFile = Dir ++ "/lingd_script.erl",
			{ok, Module, Binary} = compile:file(ScriptFile, [verbose,report_errors, report_warnings, binary]),
			{module, Module} = code:load_binary(Module, ScriptFile, Binary),
			DomConf = Module:start(Node, Domain, Img, extra(Node), Yml),
			file:write_file("/io/" ++ Node ++ "/ctl", DomConf),
			timer:sleep(2000)
		end,
		filelib:wildcard("/io/" ++ Node ++ "/*/*.{yaml,yml}")
	).

extra(Node) ->
	Bridge = filelib:wildcard("/io/" ++ Node ++ "/bridge/*/ipaddr"),
	Run = filelib:wildcard("/io/" ++ Node ++ "/run/*/ipaddr"),
	UsedIp = 
		lists:map(
			fun(F) ->
				{ok, Line} = file:read_file(F),
				[A,B,C,D] = string:tokens(binary_to_list(Line), ".\n"),
				{list_to_integer(A), list_to_integer(B), list_to_integer(C), list_to_integer(D)}
			end,
			Bridge ++ Run
		),
	Gateway = hd(UsedIp),
	{A,B,C,D} = lists:last(UsedIp),

	"-ipaddr " ++ ipaddr_to_list({A,B,C,D+1}) ++
	" -netmask 255.255.255.0 -gateway " ++ ipaddr_to_list(Gateway).

ipaddr_to_list({A,B,C,D}) ->
	lists:flatten(io_lib:format("~p.~p.~p.~p", [A,B,C,D])).

eth0_ipaddr() ->
	{ok, IfAddrs} = inet:getifaddrs(),
	Eth0 = proplists:get_value("eth0", IfAddrs),
	{addr, Addr} = lists:keyfind(addr, 1, Eth0),
	ipaddr_to_list(Addr).

store_string(File, String) ->
	ok = filelib:ensure_dir(File),
	ok = file:write_file(File, String ++ "\n").
