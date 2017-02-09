//#include <Contour.ddl>

Contour Data=
 {
  {
   { { 'A' , True , False , True } , 0 , &Pad0 },
   { { 'h' , True , True , True } , 1 , &Pad1 }
  },
  {
  { { 'c' , True , True , True } , 0 , &F0 },
  { { 'b' , True , True , True } , 1 , &F1 },
  { { 'r' , True , True , True } , 2 , &F2 },
  { { 'B' , True , False , True } , 3 , &F3 },
  { { 'C' , True , False , True } , 4 , &F4 },
  { { 'exclam' , True , False , False } , 5 , &F5 },
  { { 'D' , True , False , True } , 6 , &F6 },
  { { 'E' , True , False , True } , 7 , &F7 },
  { { 'cir' , True , False , False } , 8 , &F8 }
  }
 };

Point Pad0={{5B80000000000000h,-53},{6400000000000000h,-57},0};

Length Pad1={{4B00000000000000h,-53},0};

Div F0={Data.pads+1,&F10};

Ratio F10={{6000000000000000h,-60},0};

Add F1={&F11,&F12};

Mul F11={&F13,Data.formulas+0};

Ratio F13={{4000000000000000h,-60},0};

Div F12={Data.formulas+0,&F16};

Ratio F16={{4000000000000000h,-61},0};

Mul F2={&F17,Data.formulas+0};

Ratio F17={{6000000000000000h,-63},0};

Down F3={Data.pads+1,Data.pads+0};

Left F4={Data.formulas+0,&F22};

Down F22={Data.formulas+0,Data.pads+0};

BLoopOf F5={{&F25}};

StepOf F25={{Data.formulas+7,Data.formulas+4,Data.pads+0,Data.formulas+6}};

Right F6={Data.formulas+0,&F31};

Down F31={Data.formulas+0,Data.pads+0};

Down F7={Data.formulas+1,Data.pads+0};

CircleOf F8={&F36,Data.formulas+2};

Down F36={Data.formulas+2,Data.formulas+7};

Loop Contour_exclam=
 {
  {
   {{{5B80000000000000h,-53},{7D00000000000000h,-54},0},True},
   {{{4F00000000000000h,-53},{4B00000000000000h,-55},0},False},
   {{{5B80000000000000h,-53},{6400000000000000h,-57},0},False},
   {{{6800000000000000h,-53},{4B00000000000000h,-55},0},False}
  },
  0
 };

/*

SmoothDot Contour_exclam[]=
 {
  {{749568,512000},Smooth::DotBreak},
  {{647168,153600},Smooth::DotSimple},
  {{749568,51200},Smooth::DotSimple},
  {{851968,153600},Smooth::DotSimple}
 };

*/

