simple Tic
{
    parameters:
        @display("i=block/routing");
    gates:
        output out;
}
simple Shaper
{
    parameters:
        @display("i=block/routing");
    gates:
        input in;
        output out;
}
simple Toc
{
    parameters:
        @display("i=block/process");
    gates:
        input in;
}
network tokenbucket
{
    @display("bgb=475.41,155.82");
    submodules:
        tic: Tic {
            parameters:
                @display("i=,cyan;p=41,61");
        }
        shaper: Shaper {
            parameters:
                @display("i=,gold;p=225.94,60.774998");
        }
        toc: Toc {
            parameters:
                @display("i=,cyan;p=407.55,60.774998");
        }
    connections:
        tic.out --> {  delay = 100ms; datarate = 100Mbps; } --> shaper.in;
        toc.in <-- {  delay = 100ms; datarate = 100Mbps; } <-- shaper.out;
}






