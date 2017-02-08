//#include <Contour.ddl>

Contour Data=
 {
  {
   { { 'A' , True , True , True } , 0 , &Pad0 },
   { { 'B' , True , True , True } , 1 , &Pad1 },
   { { 'C' , True , True , True } , 2 , &Pad2 },
   { { 'r' , True , True , True } , 3 , &Pad3 },
   { { 'a' , True , True , True } , 4 , &Pad4 }
  },
  {
  { { 'E' , True , True , True } , 0 , &F0 },
  { { 'AB' , True , True , True } , 1 , &F1 },
  { { 's' , True , True , True } , 2 , &F2 },
  { { 'm' , True , True , True } , 3 , &F3 }
  }
 };

Point Pad0={{5C00000000000000h,-54},{63C0000000000000h,-54},0};

Point Pad1={{4FA0000000000000h,-53},{6A00000000000000h,-55},0};

Point Pad2={{48A0000000000000h,-53},{6440000000000000h,-54},0};

Ratio Pad3={{4000000000000000h,-61},0};

Angle Pad4={{5C31E7D9BB386800h,-63},0};

CircleOuter F0={Data.pads+0,Data.pads+1,Data.pads+2};

LineOf F1={Data.pads+0,Data.pads+1};

Meet F2={Data.formulas+3,&F10};

LineOf F10={Data.pads+0,Data.pads+1};

LineOf F3={Data.pads+2,&F14};

Rotate F14={Data.pads+0,&F16,Data.pads+1};

Mul F16={Data.pads+3,Data.pads+4};

