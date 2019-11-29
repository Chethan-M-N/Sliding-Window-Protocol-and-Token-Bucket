
simple Tic1
{
    parameters:
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

simple Toc1
{
    parameters:
        @display("i=block/process");
        int window;
        
    gates:
        input in;
        output out;
}

network Tictoc1
{
    @display("bgb=274,139");
    submodules:
        tic: Tic1 {
            parameters:
                @display("i=,cyan;p=41,61");
        }
        toc: Toc1 {
            parameters:
                @display("i=,gold;p=223,61");
                
        }
    connections:
        tic.out --> {  delay = 100ms; datarate = 100Mbps;} --> toc.in;
        tic.in <-- {  delay = 100ms; datarate = 100Mbps;} <-- toc.out;
}