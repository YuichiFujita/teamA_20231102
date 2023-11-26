xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 177;
 -0.03078;7.48274;-0.06817;,
 0.91316;7.07870;-1.86670;,
 -0.03078;7.07870;-2.09936;,
 -0.03078;7.48274;-0.06817;,
 1.64085;7.07870;-1.22202;,
 -0.03078;7.48274;-0.06817;,
 1.98560;7.07870;-0.31301;,
 -0.03078;7.48274;-0.06817;,
 1.86841;7.07870;0.65209;,
 -0.03078;7.48274;-0.06817;,
 1.31615;7.07870;1.45219;,
 -0.03078;7.48274;-0.06817;,
 0.45531;7.07870;1.90399;,
 -0.03078;7.48274;-0.06817;,
 -0.51687;7.07870;1.90399;,
 -0.03078;7.48274;-0.06817;,
 -1.37771;7.07870;1.45219;,
 -0.03078;7.48274;-0.06817;,
 -1.92997;7.07870;0.65209;,
 -0.03078;7.48274;-0.06817;,
 -2.04716;7.07870;-0.31301;,
 -0.03078;7.48274;-0.06817;,
 -1.70241;7.07870;-1.22202;,
 -0.03078;7.48274;-0.06817;,
 -0.97472;7.07870;-1.86670;,
 -0.03078;7.48274;-0.06817;,
 -0.03078;7.07870;-2.09936;,
 1.71339;5.92812;-3.39142;,
 -0.03078;5.92812;-3.82132;,
 3.05799;5.92812;-2.20020;,
 3.69500;5.92812;-0.52056;,
 3.47847;5.92812;1.26271;,
 2.45801;5.92812;2.74109;,
 0.86741;5.92812;3.57591;,
 -0.92897;5.92812;3.57591;,
 -2.51957;5.92812;2.74109;,
 -3.54003;5.92812;1.26271;,
 -3.75656;5.92812;-0.52057;,
 -3.11955;5.92812;-2.20020;,
 -1.77495;5.92812;-3.39142;,
 -0.03078;5.92812;-3.82132;,
 1.83627;3.69092;-3.58662;,
 -0.03078;3.69092;-4.14831;,
 3.34216;3.69092;-2.44278;,
 4.05980;4.20615;-0.65925;,
 3.77689;4.20615;1.34473;,
 2.60650;4.20615;2.91242;,
 1.14276;4.20615;4.29712;,
 -1.20432;4.20615;4.29712;,
 -2.70853;4.20615;3.01288;,
 -3.83818;4.20615;1.36061;,
 -4.12109;4.20615;-0.65925;,
 -3.28880;3.69092;-2.51274;,
 -1.80707;3.69092;-3.58662;,
 -0.03078;3.69092;-4.14831;,
 0.97233;2.91929;-3.94437;,
 -0.03078;2.91929;-4.14023;,
 3.67467;2.91929;-2.67230;,
 4.46088;2.17498;-0.70795;,
 4.15466;2.17498;1.48800;,
 2.87442;2.17498;3.21484;,
 1.23945;2.17498;4.68941;,
 -1.30101;2.17498;4.68941;,
 -2.97645;2.17498;3.31530;,
 -4.21595;2.17498;1.50388;,
 -4.52217;2.17498;-0.70795;,
 -3.62131;2.91929;-2.74225;,
 -0.87079;2.91929;-3.94437;,
 -0.03078;2.91929;-4.14023;,
 1.06281;0.14380;-4.41020;,
 -0.03078;0.14380;-4.55627;,
 4.00490;0.14380;-2.85380;,
 4.83718;0.14380;-0.65925;,
 4.55427;0.14380;1.67071;,
 3.22098;0.14380;3.60231;,
 1.14276;0.14380;4.69305;,
 -1.20432;0.14380;4.69305;,
 -3.28254;0.14380;3.60231;,
 -4.61583;0.14380;1.67071;,
 -4.89874;0.14380;-0.65925;,
 -4.06646;0.14380;-2.85380;,
 -0.85170;0.14380;-4.41020;,
 -0.03078;0.14380;-4.55627;,
 1.71339;0.08355;-3.39142;,
 -0.03078;0.08355;-3.82132;,
 3.05799;0.08355;-2.20020;,
 3.69500;0.08355;-0.52056;,
 3.47847;0.08355;1.26271;,
 2.45801;0.08355;2.74109;,
 0.86741;0.08355;3.57591;,
 -0.92897;0.08355;3.57591;,
 -2.51957;0.08355;2.74109;,
 -3.54003;0.08355;1.26271;,
 -3.75656;0.08355;-0.52057;,
 -3.11955;0.08355;-2.20020;,
 -1.77495;0.08355;-3.39142;,
 -0.03078;0.08355;-3.82132;,
 0.91316;0.10758;-1.86670;,
 -0.03078;0.10758;-2.09936;,
 1.64085;0.10758;-1.22202;,
 1.98560;0.10758;-0.31301;,
 1.86841;0.10758;0.65209;,
 1.31615;0.10758;1.45219;,
 0.45531;0.10758;1.90399;,
 -0.51687;0.10758;1.90399;,
 -1.37771;0.10758;1.45219;,
 -1.92997;0.10758;0.65209;,
 -2.04716;0.10758;-0.31301;,
 -1.70241;0.10758;-1.22202;,
 -0.97472;0.10758;-1.86670;,
 -0.03078;0.10758;-2.09936;,
 -0.03078;0.10395;-0.06817;,
 -0.03078;0.10395;-0.06817;,
 -0.03078;0.10395;-0.06817;,
 -0.03078;0.10395;-0.06817;,
 -0.03078;0.10395;-0.06817;,
 -0.03078;0.10395;-0.06817;,
 -0.03078;0.10395;-0.06817;,
 -0.03078;0.10395;-0.06817;,
 -0.03078;0.10395;-0.06817;,
 -0.03078;0.10395;-0.06817;,
 -0.03078;0.10395;-0.06817;,
 -0.03078;0.10395;-0.06817;,
 -0.03078;0.10395;-0.06817;,
 0.15617;1.88332;9.25462;,
 0.14495;2.81013;8.51492;,
 0.91074;3.12730;8.50938;,
 0.93281;2.21399;9.23977;,
 1.22795;3.89297;8.49602;,
 1.24794;2.99674;9.21210;,
 0.91074;4.65866;8.48265;,
 0.91694;3.77303;9.18784;,
 0.14495;4.97581;8.47712;,
 0.13373;4.08809;9.18120;,
 -0.62085;4.65866;8.48265;,
 -0.64292;3.75743;9.19605;,
 -0.93805;3.89297;8.49602;,
 -0.95805;2.97468;9.22371;,
 -0.62085;3.12730;8.50938;,
 -0.62705;2.19840;9.24797;,
 0.14495;2.81013;8.51492;,
 0.15617;1.88332;9.25462;,
 0.14495;3.61351;7.26194;,
 0.90581;3.92862;7.25645;,
 1.22097;4.68937;7.24318;,
 0.90581;5.45012;7.22988;,
 0.14495;5.76522;7.22438;,
 -0.61592;5.45012;7.22988;,
 -0.93108;4.68937;7.24318;,
 -0.61592;3.92862;7.25645;,
 0.14495;3.61351;7.26194;,
 0.14495;4.69264;5.54324;,
 0.83366;4.97788;5.53827;,
 1.11893;5.66649;5.52625;,
 0.83366;6.35509;5.51423;,
 0.14495;6.64033;5.50925;,
 -0.54377;6.35509;5.51423;,
 -0.82904;5.66649;5.52625;,
 -0.54377;4.97788;5.53827;,
 0.14495;4.69264;5.54324;,
 0.14495;5.63249;3.57857;,
 0.14495;5.63249;3.57857;,
 0.14495;5.63249;3.57857;,
 0.14495;5.63249;3.57857;,
 0.14495;5.63249;3.57857;,
 0.14495;5.63249;3.57857;,
 0.14495;5.63249;3.57857;,
 0.14495;5.63249;3.57857;,
 0.14495;2.98572;9.21791;,
 0.15617;1.88332;9.25462;,
 0.93281;2.21399;9.23977;,
 1.24794;2.99674;9.21210;,
 0.91694;3.77303;9.18784;,
 0.13373;4.08809;9.18120;,
 -0.64292;3.75743;9.19605;,
 -0.95805;2.97468;9.22371;,
 -0.62705;2.19840;9.24797;;
 
 144;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 3;17,18,16;,
 3;19,20,18;,
 3;21,22,20;,
 3;23,24,22;,
 3;25,26,24;,
 4;2,1,27,28;,
 4;1,4,29,27;,
 4;4,6,30,29;,
 4;6,8,31,30;,
 4;8,10,32,31;,
 4;10,12,33,32;,
 4;12,14,34,33;,
 4;14,16,35,34;,
 4;16,18,36,35;,
 4;18,20,37,36;,
 4;20,22,38,37;,
 4;22,24,39,38;,
 4;24,26,40,39;,
 4;28,27,41,42;,
 4;27,29,43,41;,
 4;29,30,44,43;,
 4;30,31,45,44;,
 4;31,32,46,45;,
 4;32,33,47,46;,
 4;33,34,48,47;,
 4;34,35,49,48;,
 4;35,36,50,49;,
 4;36,37,51,50;,
 4;37,38,52,51;,
 4;38,39,53,52;,
 4;39,40,54,53;,
 4;42,41,55,56;,
 4;41,43,57,55;,
 4;43,44,58,57;,
 4;44,45,59,58;,
 4;45,46,60,59;,
 4;46,47,61,60;,
 4;47,48,62,61;,
 4;48,49,63,62;,
 4;49,50,64,63;,
 4;50,51,65,64;,
 4;51,52,66,65;,
 4;52,53,67,66;,
 4;53,54,68,67;,
 4;56,55,69,70;,
 4;55,57,71,69;,
 4;57,58,72,71;,
 4;58,59,73,72;,
 4;59,60,74,73;,
 4;60,61,75,74;,
 4;61,62,76,75;,
 4;62,63,77,76;,
 4;63,64,78,77;,
 4;64,65,79,78;,
 4;65,66,80,79;,
 4;66,67,81,80;,
 4;67,68,82,81;,
 4;70,69,83,84;,
 4;69,71,85,83;,
 4;71,72,86,85;,
 4;72,73,87,86;,
 4;73,74,88,87;,
 4;74,75,89,88;,
 4;75,76,90,89;,
 4;76,77,91,90;,
 4;77,78,92,91;,
 4;78,79,93,92;,
 4;79,80,94,93;,
 4;80,81,95,94;,
 4;81,82,96,95;,
 4;84,83,97,98;,
 4;83,85,99,97;,
 4;85,86,100,99;,
 4;86,87,101,100;,
 4;87,88,102,101;,
 4;88,89,103,102;,
 4;89,90,104,103;,
 4;90,91,105,104;,
 4;91,92,106,105;,
 4;92,93,107,106;,
 4;93,94,108,107;,
 4;94,95,109,108;,
 4;95,96,110,109;,
 3;98,97,111;,
 3;97,99,112;,
 3;99,100,113;,
 3;100,101,114;,
 3;101,102,115;,
 3;102,103,116;,
 3;103,104,117;,
 3;104,105,118;,
 3;105,106,119;,
 3;106,107,120;,
 3;107,108,121;,
 3;108,109,122;,
 3;109,110,123;,
 4;124,125,126,127;,
 4;127,126,128,129;,
 4;129,128,130,131;,
 4;131,130,132,133;,
 4;133,132,134,135;,
 4;135,134,136,137;,
 4;137,136,138,139;,
 4;139,138,140,141;,
 4;125,142,143,126;,
 4;126,143,144,128;,
 4;128,144,145,130;,
 4;130,145,146,132;,
 4;132,146,147,134;,
 4;134,147,148,136;,
 4;136,148,149,138;,
 4;138,149,150,140;,
 4;142,151,152,143;,
 4;143,152,153,144;,
 4;144,153,154,145;,
 4;145,154,155,146;,
 4;146,155,156,147;,
 4;147,156,157,148;,
 4;148,157,158,149;,
 4;149,158,159,150;,
 3;151,160,152;,
 3;152,161,153;,
 3;153,162,154;,
 3;154,163,155;,
 3;155,164,156;,
 3;156,165,157;,
 3;157,166,158;,
 3;158,167,159;,
 3;168,169,170;,
 3;168,170,171;,
 3;168,171,172;,
 3;168,172,173;,
 3;168,173,174;,
 3;168,174,175;,
 3;168,175,176;,
 3;168,176,169;;
 
 MeshMaterialList {
  4;
  144;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  2,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.000000;0.024800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.542400;0.542400;0.542400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.279200;0.279200;0.279200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  150;
  0.000000;1.000000;0.000000;,
  0.000000;0.924187;-0.381939;,
  0.177497;0.924187;-0.338190;,
  0.314330;0.924187;-0.216967;,
  0.379154;0.924187;-0.046038;,
  0.357120;0.924187;0.135438;,
  0.253273;0.924187;0.285886;,
  0.091404;0.924187;0.370841;,
  -0.091404;0.924187;0.370841;,
  -0.253273;0.924187;0.285885;,
  -0.357120;0.924187;0.135438;,
  -0.379154;0.924187;-0.046038;,
  -0.314330;0.924187;-0.216967;,
  -0.177497;0.924187;-0.338190;,
  -0.001927;0.528801;-0.848743;,
  0.391279;0.529191;-0.752900;,
  0.678071;0.546765;-0.491190;,
  0.822273;0.559211;-0.105597;,
  0.778790;0.548791;0.303835;,
  0.571704;0.561170;0.598534;,
  0.222168;0.620480;0.752094;,
  -0.214480;0.625961;0.749781;,
  -0.564597;0.575114;0.592009;,
  -0.777489;0.554372;0.296955;,
  -0.823467;0.556592;-0.110037;,
  -0.677354;0.540230;-0.499343;,
  -0.391620;0.524334;-0.756114;,
  -0.008163;0.082713;-0.996540;,
  0.416043;0.129021;-0.900145;,
  0.769621;0.219610;-0.599546;,
  0.967961;0.226876;-0.107606;,
  0.911730;0.182920;0.367817;,
  0.711052;0.196617;0.675090;,
  0.324990;0.270478;0.906214;,
  -0.306925;0.277777;0.910295;,
  -0.706652;0.213456;0.674596;,
  -0.916221;0.189725;0.352906;,
  -0.966953;0.224361;-0.121096;,
  -0.757414;0.211246;-0.617818;,
  -0.415781;0.117066;-0.901899;,
  -0.016289;0.102411;-0.994609;,
  0.340702;0.140125;-0.929671;,
  0.742785;0.214944;-0.634090;,
  0.971202;0.217462;-0.097353;,
  0.908177;0.205100;0.364894;,
  0.710785;0.180227;0.679929;,
  0.327811;0.130499;0.935687;,
  -0.312266;0.121448;0.942199;,
  -0.710492;0.167507;0.683478;,
  -0.914708;0.198644;0.351924;,
  -0.968742;0.222279;-0.110138;,
  -0.724637;0.215756;-0.654486;,
  -0.343140;0.129207;-0.930355;,
  -0.019720;0.157957;-0.987249;,
  0.304709;0.149668;-0.940613;,
  0.737642;0.153621;-0.657483;,
  0.978593;0.175069;-0.108200;,
  0.908962;0.214726;0.357325;,
  0.690250;0.172690;0.702662;,
  0.297104;0.061619;0.952855;,
  -0.288080;0.043949;0.956597;,
  -0.692787;0.145082;0.706398;,
  -0.914577;0.202729;0.349929;,
  -0.976419;0.182739;-0.114947;,
  -0.721480;0.156328;-0.674558;,
  -0.308826;0.141129;-0.940590;,
  -0.000636;-0.999496;-0.031723;,
  0.010633;-0.999524;-0.028967;,
  0.017204;-0.999708;-0.016976;,
  0.019039;-0.999816;-0.002312;,
  0.017933;-0.999816;0.006801;,
  0.012718;-0.999816;0.014356;,
  0.004590;-0.999816;0.018622;,
  -0.004590;-0.999816;0.018622;,
  -0.012718;-0.999816;0.014356;,
  -0.017933;-0.999816;0.006801;,
  -0.019039;-0.999816;-0.002312;,
  -0.017667;-0.999664;-0.018960;,
  -0.011731;-0.999429;-0.031692;,
  -0.000000;-0.999982;0.006083;,
  -0.002827;-0.999982;0.005386;,
  -0.005006;-0.999982;0.003456;,
  -0.006039;-0.999982;0.000733;,
  -0.005688;-0.999982;-0.002157;,
  -0.004034;-0.999982;-0.004553;,
  -0.001456;-0.999982;-0.005907;,
  0.001456;-0.999982;-0.005907;,
  0.004034;-0.999982;-0.004553;,
  0.005688;-0.999982;-0.002157;,
  0.006039;-0.999982;0.000733;,
  0.005006;-0.999982;0.003456;,
  0.002827;-0.999982;0.005386;,
  -0.000000;-1.000000;0.000000;,
  0.001079;-0.622436;-0.782670;,
  0.566805;-0.506624;-0.649665;,
  0.818963;-0.346572;-0.457370;,
  0.305710;0.291858;0.906289;,
  0.001635;0.340860;0.940113;,
  -0.305266;0.292153;0.906344;,
  -0.835921;0.352518;0.420675;,
  -0.562708;-0.511055;-0.649756;,
  0.000555;-0.740692;-0.671845;,
  0.619203;-0.575101;-0.534646;,
  0.976115;-0.129315;-0.174579;,
  0.622571;0.580504;0.524804;,
  -0.000989;0.743419;0.668826;,
  -0.626475;0.579793;0.520931;,
  -0.979370;0.131686;0.153274;,
  -0.617074;-0.577301;-0.534737;,
  0.000002;-0.844371;-0.535760;,
  0.657372;-0.632124;-0.410222;,
  0.999508;0.002186;-0.031289;,
  0.671476;0.648022;0.359426;,
  0.000002;0.869217;0.494431;,
  -0.671475;0.648022;0.359427;,
  -0.999508;0.002186;-0.031287;,
  -0.657372;-0.632124;-0.410221;,
  0.000002;-0.875828;-0.482623;,
  0.644030;-0.636156;-0.424889;,
  0.965241;-0.002091;-0.261352;,
  0.712478;0.697244;-0.078908;,
  0.000003;0.999991;-0.004247;,
  -0.712476;0.697245;-0.078909;,
  -0.965241;-0.002091;-0.261354;,
  -0.644028;-0.636157;-0.424891;,
  0.632464;-0.640157;-0.436106;,
  0.004925;0.033334;0.999432;,
  -0.001270;-0.997004;-0.077338;,
  0.027731;-0.997145;-0.070236;,
  0.045875;-0.998070;-0.041861;,
  0.051912;-0.998632;-0.006303;,
  0.048895;-0.998632;0.018543;,
  0.034677;-0.998632;0.039142;,
  0.012515;-0.998632;0.050773;,
  -0.012515;-0.998632;0.050773;,
  -0.034677;-0.998632;0.039142;,
  -0.048895;-0.998632;0.018543;,
  -0.051912;-0.998632;-0.006303;,
  -0.046801;-0.997853;-0.045826;,
  -0.029921;-0.996684;-0.075670;,
  0.826810;0.358083;0.433776;,
  -0.814489;-0.353542;-0.460016;,
  0.632461;0.624550;-0.458181;,
  -0.632459;0.624551;-0.458183;,
  -0.632461;-0.640158;-0.436109;,
  0.004921;0.033332;0.999432;,
  0.004925;0.033338;0.999432;,
  0.004931;0.033340;0.999432;,
  0.004922;0.033334;0.999432;,
  0.004922;0.033332;0.999432;;
  144;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,9,8;,
  3;0,10,9;,
  3;0,11,10;,
  3;0,12,11;,
  3;0,13,12;,
  3;0,1,13;,
  4;1,2,15,14;,
  4;2,3,16,15;,
  4;3,4,17,16;,
  4;4,5,18,17;,
  4;5,6,19,18;,
  4;6,7,20,19;,
  4;7,8,21,20;,
  4;8,9,22,21;,
  4;9,10,23,22;,
  4;10,11,24,23;,
  4;11,12,25,24;,
  4;12,13,26,25;,
  4;13,1,14,26;,
  4;14,15,28,27;,
  4;15,16,29,28;,
  4;16,17,30,29;,
  4;17,18,31,30;,
  4;18,19,32,31;,
  4;19,20,33,32;,
  4;20,21,34,33;,
  4;21,22,35,34;,
  4;22,23,36,35;,
  4;23,24,37,36;,
  4;24,25,38,37;,
  4;25,26,39,38;,
  4;26,14,27,39;,
  4;27,28,41,40;,
  4;28,29,42,41;,
  4;29,30,43,42;,
  4;30,31,44,43;,
  4;31,32,45,44;,
  4;32,33,46,45;,
  4;33,34,47,46;,
  4;34,35,48,47;,
  4;35,36,49,48;,
  4;36,37,50,49;,
  4;37,38,51,50;,
  4;38,39,52,51;,
  4;39,27,40,52;,
  4;40,41,54,53;,
  4;41,42,55,54;,
  4;42,43,56,55;,
  4;43,44,57,56;,
  4;44,45,58,57;,
  4;45,46,59,58;,
  4;46,47,60,59;,
  4;47,48,61,60;,
  4;48,49,62,61;,
  4;49,50,63,62;,
  4;50,51,64,63;,
  4;51,52,65,64;,
  4;52,40,53,65;,
  4;127,128,67,66;,
  4;128,129,68,67;,
  4;129,130,69,68;,
  4;130,131,70,69;,
  4;131,132,71,70;,
  4;132,133,72,71;,
  4;133,134,73,72;,
  4;134,135,74,73;,
  4;135,136,75,74;,
  4;136,137,76,75;,
  4;137,138,77,76;,
  4;138,139,78,77;,
  4;139,127,66,78;,
  4;66,67,80,79;,
  4;67,68,81,80;,
  4;68,69,82,81;,
  4;69,70,83,82;,
  4;70,71,84,83;,
  4;71,72,85,84;,
  4;72,73,86,85;,
  4;73,74,87,86;,
  4;74,75,88,87;,
  4;75,76,89,88;,
  4;76,77,90,89;,
  4;77,78,91,90;,
  4;78,66,79,91;,
  3;79,80,92;,
  3;80,81,92;,
  3;81,82,92;,
  3;82,83,92;,
  3;83,84,92;,
  3;84,85,92;,
  3;85,86,92;,
  3;86,87,92;,
  3;87,88,92;,
  3;88,89,92;,
  3;89,90,92;,
  3;90,91,92;,
  3;91,79,92;,
  4;93,101,102,94;,
  4;94,102,103,95;,
  4;140,140,104,96;,
  4;96,104,105,97;,
  4;97,105,106,98;,
  4;98,106,107,99;,
  4;141,141,108,100;,
  4;100,108,101,93;,
  4;101,109,110,102;,
  4;102,110,111,103;,
  4;103,111,112,104;,
  4;104,112,113,105;,
  4;105,113,114,106;,
  4;106,114,115,107;,
  4;107,115,116,108;,
  4;108,116,109,101;,
  4;109,117,118,110;,
  4;110,118,119,111;,
  4;111,119,120,112;,
  4;112,120,121,113;,
  4;113,121,122,114;,
  4;114,122,123,115;,
  4;115,123,124,116;,
  4;116,124,117,109;,
  3;117,125,118;,
  3;118,125,119;,
  3;119,142,120;,
  3;120,142,121;,
  3;121,143,122;,
  3;122,143,123;,
  3;123,144,124;,
  3;124,144,117;,
  3;126,145,146;,
  3;126,146,147;,
  3;126,147,96;,
  3;126,96,97;,
  3;126,97,98;,
  3;126,98,148;,
  3;126,148,149;,
  3;126,149,145;;
 }
 MeshTextureCoords {
  177;
  0.038460;0.000000;,
  0.076920;0.125000;,
  0.000000;0.125000;,
  0.115380;0.000000;,
  0.153850;0.125000;,
  0.192310;0.000000;,
  0.230770;0.125000;,
  0.269230;0.000000;,
  0.307690;0.125000;,
  0.346150;0.000000;,
  0.384620;0.125000;,
  0.423080;0.000000;,
  0.461540;0.125000;,
  0.500000;0.000000;,
  0.538460;0.125000;,
  0.576920;0.000000;,
  0.615380;0.125000;,
  0.653850;0.000000;,
  0.692310;0.125000;,
  0.730770;0.000000;,
  0.769230;0.125000;,
  0.807690;0.000000;,
  0.846150;0.125000;,
  0.884620;0.000000;,
  0.923080;0.125000;,
  0.961540;0.000000;,
  1.000000;0.125000;,
  0.076920;0.250000;,
  0.000000;0.250000;,
  0.153850;0.250000;,
  0.230770;0.250000;,
  0.307690;0.250000;,
  0.384620;0.250000;,
  0.461540;0.250000;,
  0.538460;0.250000;,
  0.615380;0.250000;,
  0.692310;0.250000;,
  0.769230;0.250000;,
  0.846150;0.250000;,
  0.923080;0.250000;,
  1.000000;0.250000;,
  0.076920;0.375000;,
  0.000000;0.375000;,
  0.153850;0.375000;,
  0.230770;0.375000;,
  0.307690;0.375000;,
  0.384620;0.375000;,
  0.461540;0.375000;,
  0.538460;0.375000;,
  0.615380;0.375000;,
  0.692310;0.375000;,
  0.769230;0.375000;,
  0.846150;0.375000;,
  0.923080;0.375000;,
  1.000000;0.375000;,
  0.076920;0.500000;,
  0.000000;0.500000;,
  0.153850;0.500000;,
  0.230770;0.500000;,
  0.307690;0.500000;,
  0.384620;0.500000;,
  0.461540;0.500000;,
  0.538460;0.500000;,
  0.615380;0.500000;,
  0.692310;0.500000;,
  0.769230;0.500000;,
  0.846150;0.500000;,
  0.923080;0.500000;,
  1.000000;0.500000;,
  0.076920;0.625000;,
  0.000000;0.625000;,
  0.153850;0.625000;,
  0.230770;0.625000;,
  0.307690;0.625000;,
  0.384620;0.625000;,
  0.461540;0.625000;,
  0.538460;0.625000;,
  0.615380;0.625000;,
  0.692310;0.625000;,
  0.769230;0.625000;,
  0.846150;0.625000;,
  0.923080;0.625000;,
  1.000000;0.625000;,
  0.076920;0.750000;,
  0.000000;0.750000;,
  0.153850;0.750000;,
  0.230770;0.750000;,
  0.307690;0.750000;,
  0.384620;0.750000;,
  0.461540;0.750000;,
  0.538460;0.750000;,
  0.615380;0.750000;,
  0.692310;0.750000;,
  0.769230;0.750000;,
  0.846150;0.750000;,
  0.923080;0.750000;,
  1.000000;0.750000;,
  0.076920;0.875000;,
  0.000000;0.875000;,
  0.153850;0.875000;,
  0.230770;0.875000;,
  0.307690;0.875000;,
  0.384620;0.875000;,
  0.461540;0.875000;,
  0.538460;0.875000;,
  0.615380;0.875000;,
  0.692310;0.875000;,
  0.769230;0.875000;,
  0.846150;0.875000;,
  0.923080;0.875000;,
  1.000000;0.875000;,
  0.038460;1.000000;,
  0.115380;1.000000;,
  0.192310;1.000000;,
  0.269230;1.000000;,
  0.346150;1.000000;,
  0.423080;1.000000;,
  0.500000;1.000000;,
  0.576920;1.000000;,
  0.653850;1.000000;,
  0.730770;1.000000;,
  0.807690;1.000000;,
  0.884620;1.000000;,
  0.961540;1.000000;,
  0.000000;1.000000;,
  0.000000;0.750000;,
  0.125000;0.750000;,
  0.125000;1.000000;,
  0.250000;0.750000;,
  0.250000;1.000000;,
  0.375000;0.750000;,
  0.375000;1.000000;,
  0.500000;0.750000;,
  0.500000;1.000000;,
  0.625000;0.750000;,
  0.625000;1.000000;,
  0.750000;0.750000;,
  0.750000;1.000000;,
  0.875000;0.750000;,
  0.875000;1.000000;,
  1.000000;0.750000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.125000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.250000;,
  0.125000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.062500;0.000000;,
  0.125000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
