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
 169;
 1.45585;2.09115;-0.00061;,
 0.47271;1.81458;0.98253;,
 1.45585;1.81458;1.38976;,
 1.45585;2.09115;-0.00061;,
 0.06547;1.81458;-0.00061;,
 1.45585;2.09115;-0.00061;,
 0.47271;1.81458;-0.98375;,
 1.45585;2.09115;-0.00061;,
 1.45585;1.81458;-1.39098;,
 -0.36075;1.02701;1.81599;,
 1.45585;1.02701;2.56845;,
 -1.11322;1.02701;-0.00061;,
 -0.36075;1.02701;-1.81721;,
 1.45585;1.02701;-2.56967;,
 -0.91766;-0.15170;2.37289;,
 1.45585;-0.15170;3.35603;,
 -1.90079;-0.15170;-0.00061;,
 -0.91766;-0.15170;-2.37411;,
 1.45585;-0.15170;-3.35725;,
 0.00011;1.22070;-0.00061;,
 0.35731;1.12023;-0.35780;,
 0.00011;1.12023;-0.50576;,
 0.00011;1.22070;-0.00061;,
 0.50526;1.12023;-0.00061;,
 0.00011;1.22070;-0.00061;,
 0.35731;1.12023;0.35658;,
 0.00011;1.22070;-0.00061;,
 0.00011;1.12023;0.50454;,
 0.00011;1.22070;-0.00061;,
 -0.35708;1.12023;0.35658;,
 0.00011;1.22070;-0.00061;,
 -0.50502;1.12023;-0.00061;,
 0.00011;1.22070;-0.00061;,
 -0.35708;1.12023;-0.35780;,
 0.00011;1.22070;-0.00061;,
 0.00011;1.12023;-0.50576;,
 0.66012;0.83407;-0.66062;,
 0.00011;0.83407;-0.93400;,
 0.93351;0.83407;-0.00061;,
 0.66012;0.83407;0.65940;,
 0.00011;0.83407;0.93278;,
 -0.65990;0.83407;0.65940;,
 -0.93328;0.83407;-0.00061;,
 -0.65990;0.83407;-0.66062;,
 0.00011;0.83407;-0.93400;,
 0.86246;0.40583;-0.86295;,
 0.00011;0.40583;-1.22014;,
 1.21965;0.40583;-0.00061;,
 0.86246;0.40583;0.86173;,
 0.00011;0.40583;1.21892;,
 -0.86223;0.40583;0.86173;,
 -1.21943;0.40583;-0.00061;,
 -0.86223;0.40583;-0.86295;,
 0.00011;0.40583;-1.22014;,
 0.93351;-0.09931;-0.93400;,
 0.00011;-0.09931;-1.32062;,
 1.32013;-0.09931;-0.00061;,
 0.93351;-0.09931;0.93278;,
 0.00011;-0.09931;1.31940;,
 -0.93328;-0.09931;0.93278;,
 -1.31991;-0.09931;-0.00061;,
 -0.93328;-0.09931;-0.93400;,
 0.00011;-0.09931;-1.32062;,
 0.86246;-0.60446;-0.86295;,
 0.00011;-0.60446;-1.22014;,
 1.21965;-0.60446;-0.00061;,
 0.86246;-0.60446;0.86173;,
 0.00011;-0.60446;1.21892;,
 -0.86223;-0.60446;0.86173;,
 -1.21943;-0.60446;-0.00061;,
 -0.86223;-0.60446;-0.86295;,
 0.00011;-0.60446;-1.22014;,
 0.66012;-1.03270;-0.66062;,
 0.00011;-1.03270;-0.93400;,
 0.93351;-1.03270;-0.00061;,
 0.66012;-1.03270;0.65940;,
 0.00011;-1.03270;0.93278;,
 -0.65990;-1.03270;0.65940;,
 -0.93328;-1.03270;-0.00061;,
 -0.65990;-1.03270;-0.66062;,
 0.00011;-1.03270;-0.93400;,
 0.35731;-1.31884;-0.35780;,
 0.00011;-1.31884;-0.50576;,
 0.50526;-1.31884;-0.00061;,
 0.35731;-1.31884;0.35658;,
 0.00011;-1.31884;0.50454;,
 -0.35708;-1.31884;0.35658;,
 -0.50502;-1.31884;-0.00061;,
 -0.35708;-1.31884;-0.35780;,
 0.00011;-1.31884;-0.50576;,
 0.00011;-1.41933;-0.00061;,
 0.00011;-1.41933;-0.00061;,
 0.00011;-1.41933;-0.00061;,
 0.00011;-1.41933;-0.00061;,
 0.00011;-1.41933;-0.00061;,
 0.00011;-1.41933;-0.00061;,
 0.00011;-1.41933;-0.00061;,
 0.00011;-1.41933;-0.00061;,
 -0.00120;-1.13806;-1.17756;,
 0.49194;-1.13806;-1.06101;,
 0.49194;-3.58959;-1.06101;,
 -0.00120;-3.58959;-1.17756;,
 0.88741;-1.13806;-0.73443;,
 0.88741;-3.58959;-0.73443;,
 1.10688;-1.13806;-0.26251;,
 1.10688;-3.58959;-0.26251;,
 1.10688;-1.13806;0.26129;,
 1.10688;-3.58959;0.26129;,
 0.88741;-1.13806;0.73321;,
 0.88741;-3.58959;0.73321;,
 0.49194;-1.13806;1.05979;,
 0.49194;-3.58959;1.05979;,
 -0.00120;-1.13806;1.17634;,
 -0.00120;-3.58959;1.17634;,
 -0.49434;-1.13806;1.05979;,
 -0.49434;-3.58959;1.05979;,
 -0.88980;-1.13806;0.73321;,
 -0.88980;-3.58959;0.73321;,
 -1.10927;-1.13806;0.26129;,
 -1.10927;-3.58959;0.26129;,
 -1.10927;-1.13806;-0.26251;,
 -1.10927;-3.58959;-0.26251;,
 -0.88980;-1.13806;-0.73443;,
 -0.88980;-3.58959;-0.73443;,
 -0.49434;-1.13806;-1.06101;,
 -0.49434;-3.58959;-1.06101;,
 -0.00120;-1.13806;-1.17756;,
 -0.00120;-3.58959;-1.17756;,
 -0.00120;-1.13806;-0.00061;,
 -0.00120;-1.13806;-0.00061;,
 -0.00120;-1.13806;-0.00061;,
 -0.00120;-1.13806;-0.00061;,
 -0.00120;-1.13806;-0.00061;,
 -0.00120;-1.13806;-0.00061;,
 -0.00120;-1.13806;-0.00061;,
 -0.00120;-1.13806;-0.00061;,
 -0.00120;-1.13806;-0.00061;,
 -0.00120;-1.13806;-0.00061;,
 -0.00120;-1.13806;-0.00061;,
 -0.00120;-1.13806;-0.00061;,
 -0.00120;-1.13806;-0.00061;,
 -0.00120;-1.13806;-0.00061;,
 -0.00120;-3.58959;-0.00061;,
 -0.00120;-3.58959;-0.00061;,
 -0.00120;-3.58959;-0.00061;,
 -0.00120;-3.58959;-0.00061;,
 -0.00120;-3.58959;-0.00061;,
 -0.00120;-3.58959;-0.00061;,
 -0.00120;-3.58959;-0.00061;,
 -0.00120;-3.58959;-0.00061;,
 -0.00120;-3.58959;-0.00061;,
 -0.00120;-3.58959;-0.00061;,
 -0.00120;-3.58959;-0.00061;,
 -0.00120;-3.58959;-0.00061;,
 -0.00120;-3.58959;-0.00061;,
 -0.00120;-3.58959;-0.00061;,
 1.45585;-0.79176;-3.48457;,
 1.45585;-1.54205;-3.63381;,
 -1.11322;-1.54205;-2.56967;,
 -1.00769;-0.79176;-2.46414;,
 -1.00769;-0.79176;-2.46414;,
 -2.17736;-1.54205;-0.00061;,
 -2.02811;-0.79176;-0.00061;,
 -2.02811;-0.79176;-0.00061;,
 -1.11322;-1.54205;2.56845;,
 -1.00769;-0.79176;2.46292;,
 -1.00769;-0.79176;2.46292;,
 1.45585;-1.54205;3.63259;,
 1.45585;-0.79176;3.48335;;
 
 146;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 4;2,1,9,10;,
 4;1,4,11,9;,
 4;4,6,12,11;,
 4;6,8,13,12;,
 4;10,9,14,15;,
 4;9,11,16,14;,
 4;11,12,17,16;,
 4;12,13,18,17;,
 3;2,1,0;,
 3;1,4,3;,
 3;4,6,5;,
 3;6,8,7;,
 4;10,9,1,2;,
 4;9,11,4,1;,
 4;11,12,6,4;,
 4;12,13,8,6;,
 4;15,14,9,10;,
 4;14,16,11,9;,
 4;16,17,12,11;,
 4;17,18,13,12;,
 3;19,20,21;,
 3;22,23,20;,
 3;24,25,23;,
 3;26,27,25;,
 3;28,29,27;,
 3;30,31,29;,
 3;32,33,31;,
 3;34,35,33;,
 4;21,20,36,37;,
 4;20,23,38,36;,
 4;23,25,39,38;,
 4;25,27,40,39;,
 4;27,29,41,40;,
 4;29,31,42,41;,
 4;31,33,43,42;,
 4;33,35,44,43;,
 4;37,36,45,46;,
 4;36,38,47,45;,
 4;38,39,48,47;,
 4;39,40,49,48;,
 4;40,41,50,49;,
 4;41,42,51,50;,
 4;42,43,52,51;,
 4;43,44,53,52;,
 4;46,45,54,55;,
 4;45,47,56,54;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 4;52,53,62,61;,
 4;55,54,63,64;,
 4;54,56,65,63;,
 4;56,57,66,65;,
 4;57,58,67,66;,
 4;58,59,68,67;,
 4;59,60,69,68;,
 4;60,61,70,69;,
 4;61,62,71,70;,
 4;64,63,72,73;,
 4;63,65,74,72;,
 4;65,66,75,74;,
 4;66,67,76,75;,
 4;67,68,77,76;,
 4;68,69,78,77;,
 4;69,70,79,78;,
 4;70,71,80,79;,
 4;73,72,81,82;,
 4;72,74,83,81;,
 4;74,75,84,83;,
 4;75,76,85,84;,
 4;76,77,86,85;,
 4;77,78,87,86;,
 4;78,79,88,87;,
 4;79,80,89,88;,
 3;82,81,90;,
 3;81,83,91;,
 3;83,84,92;,
 3;84,85,93;,
 3;85,86,94;,
 3;86,87,95;,
 3;87,88,96;,
 3;88,89,97;,
 4;98,99,100,101;,
 4;99,102,103,100;,
 4;102,104,105,103;,
 4;104,106,107,105;,
 4;106,108,109,107;,
 4;108,110,111,109;,
 4;110,112,113,111;,
 4;112,114,115,113;,
 4;114,116,117,115;,
 4;116,118,119,117;,
 4;118,120,121,119;,
 4;120,122,123,121;,
 4;122,124,125,123;,
 4;124,126,127,125;,
 3;128,99,98;,
 3;129,102,99;,
 3;130,104,102;,
 3;131,106,104;,
 3;132,108,106;,
 3;133,110,108;,
 3;134,112,110;,
 3;135,114,112;,
 3;136,116,114;,
 3;137,118,116;,
 3;138,120,118;,
 3;139,122,120;,
 3;140,124,122;,
 3;141,126,124;,
 3;142,101,100;,
 3;143,100,103;,
 3;144,103,105;,
 3;145,105,107;,
 3;146,107,109;,
 3;147,109,111;,
 3;148,111,113;,
 3;149,113,115;,
 3;150,115,117;,
 3;151,117,119;,
 3;152,119,121;,
 3;153,121,123;,
 3;154,123,125;,
 3;155,125,127;,
 4;156,157,158,159;,
 4;156,159,17,18;,
 4;156,18,17,160;,
 4;156,160,158,157;,
 4;159,158,161,162;,
 4;159,162,16,17;,
 4;160,17,16,163;,
 4;160,163,161,158;,
 4;162,161,164,165;,
 4;162,165,14,16;,
 4;163,16,14,166;,
 4;163,166,164,161;,
 4;165,164,167,168;,
 4;165,168,15,14;,
 4;166,14,15,168;,
 4;166,168,167,164;;
 
 MeshMaterialList {
  4;
  146;
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
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
  1,
  1,
  1,
  1,
  1,
  0,
  1,
  1,
  1,
  0,
  1,
  1,
  1,
  0,
  1,
  1,
  1,
  0,
  1,
  1,
  1;;
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
  126;
  -0.139284;0.990253;0.000000;,
  -0.155731;0.913453;-0.375968;,
  -0.155731;0.913453;0.375968;,
  -0.269132;0.924735;0.269134;,
  -0.380611;0.924735;0.000000;,
  -0.269132;0.924735;-0.269134;,
  -0.280305;0.680797;-0.676716;,
  -0.280305;0.680797;0.676716;,
  -0.498494;0.709229;0.498496;,
  -0.704978;0.709229;0.000000;,
  -0.498494;0.709229;-0.498496;,
  -0.357145;0.359190;-0.862224;,
  -0.357145;0.359190;0.862224;,
  -0.652733;0.384551;0.652732;,
  -0.923104;0.384551;0.000000;,
  -0.652733;0.384551;-0.652733;,
  -0.376380;0.180743;-0.908664;,
  -0.376380;0.180743;0.908664;,
  -0.693520;0.195092;0.693520;,
  -0.980784;0.195095;0.000000;,
  -0.693520;0.195092;-0.693520;,
  -0.000002;1.000000;-0.000000;,
  -0.000001;0.924735;-0.380611;,
  0.269134;0.924735;-0.269133;,
  0.380613;0.924734;0.000000;,
  0.269134;0.924735;0.269133;,
  -0.000001;0.924735;0.380611;,
  -0.269136;0.924736;0.269128;,
  -0.380614;0.924734;0.000000;,
  -0.269136;0.924735;-0.269128;,
  -0.000001;0.709222;-0.704985;,
  0.498498;0.709223;-0.498500;,
  0.704983;0.709224;0.000000;,
  0.498498;0.709223;0.498500;,
  -0.000001;0.709222;0.704985;,
  -0.498499;0.709225;0.498497;,
  -0.704982;0.709225;0.000000;,
  -0.498499;0.709225;-0.498497;,
  -0.000002;0.384551;-0.923104;,
  0.652730;0.384554;-0.652734;,
  0.923102;0.384555;0.000000;,
  0.652730;0.384554;0.652734;,
  -0.000002;0.384551;0.923104;,
  -0.652730;0.384553;0.652735;,
  -0.923102;0.384555;0.000000;,
  -0.652730;0.384553;-0.652735;,
  -0.000002;0.000001;-1.000000;,
  0.707106;0.000001;-0.707108;,
  1.000000;0.000001;0.000000;,
  0.707106;0.000001;0.707108;,
  -0.000002;0.000001;1.000000;,
  -0.707104;0.000001;0.707110;,
  -1.000000;0.000001;0.000000;,
  -0.707104;0.000001;-0.707110;,
  -0.000002;-0.384550;-0.923104;,
  0.652731;-0.384552;-0.652734;,
  0.923103;-0.384552;-0.000000;,
  0.652731;-0.384552;0.652734;,
  -0.000002;-0.384549;0.923104;,
  -0.652730;-0.384552;0.652735;,
  -0.923102;-0.384554;0.000000;,
  -0.652730;-0.384552;-0.652735;,
  -0.000001;-0.709232;-0.704976;,
  0.498491;-0.709233;-0.498493;,
  0.704973;-0.709234;-0.000000;,
  0.498491;-0.709233;0.498493;,
  -0.000001;-0.709231;0.704976;,
  -0.498490;-0.709236;0.498489;,
  -0.704968;-0.709239;0.000000;,
  -0.498490;-0.709236;-0.498489;,
  -0.000001;-0.924733;-0.380618;,
  0.269138;-0.924732;-0.269137;,
  0.380617;-0.924733;-0.000000;,
  0.269138;-0.924732;0.269137;,
  -0.000001;-0.924733;0.380618;,
  -0.269139;-0.924734;0.269132;,
  -0.380616;-0.924733;0.000000;,
  -0.269139;-0.924734;-0.269132;,
  -0.000002;-1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.000000;0.000000;-1.000000;,
  0.445006;0.000000;-0.895527;,
  0.791271;0.000000;-0.611465;,
  0.976407;0.000000;-0.215938;,
  0.976407;0.000000;0.215938;,
  0.791271;0.000000;0.611465;,
  0.445006;0.000000;0.895528;,
  -0.000000;0.000000;1.000000;,
  -0.445011;0.000000;0.895525;,
  -0.791275;0.000000;0.611461;,
  -0.976407;0.000000;0.215938;,
  -0.976407;0.000000;-0.215938;,
  -0.791275;0.000000;-0.611461;,
  -0.445011;0.000000;-0.895525;,
  0.000000;-1.000000;-0.000000;,
  -0.376380;0.180747;-0.908663;,
  -0.693520;0.195095;-0.693519;,
  0.693520;-0.195095;0.693519;,
  -0.980784;0.195096;0.000000;,
  0.980784;-0.195096;-0.000000;,
  -0.693520;0.195095;0.693519;,
  0.693520;-0.195095;-0.693519;,
  -0.376380;0.180747;0.908663;,
  0.155731;-0.913453;-0.375968;,
  0.269132;-0.924735;-0.269134;,
  0.139284;-0.990253;-0.000000;,
  0.380611;-0.924735;-0.000000;,
  0.269132;-0.924735;0.269134;,
  0.155731;-0.913453;0.375968;,
  0.280305;-0.680797;-0.676716;,
  0.498494;-0.709229;-0.498496;,
  0.704978;-0.709229;-0.000000;,
  0.498494;-0.709229;0.498496;,
  0.280305;-0.680797;0.676716;,
  0.357145;-0.359190;-0.862224;,
  0.652733;-0.384551;-0.652732;,
  0.923104;-0.384551;-0.000000;,
  0.652733;-0.384551;0.652733;,
  0.357145;-0.359190;0.862224;,
  0.376380;-0.180747;0.908663;,
  0.693520;-0.195092;0.693520;,
  0.376380;-0.180743;0.908664;,
  0.980784;-0.195095;-0.000000;,
  0.693520;-0.195092;-0.693520;,
  0.376380;-0.180747;-0.908663;,
  0.376380;-0.180743;-0.908664;;
  146;
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,1,5;,
  4;2,3,8,7;,
  4;3,4,9,8;,
  4;4,5,10,9;,
  4;5,1,6,10;,
  4;7,8,13,12;,
  4;8,9,14,13;,
  4;9,10,15,14;,
  4;10,6,11,15;,
  3;103,104,105;,
  3;104,106,105;,
  3;106,107,105;,
  3;107,108,105;,
  4;109,110,104,103;,
  4;110,111,106,104;,
  4;111,112,107,106;,
  4;112,113,108,107;,
  4;114,115,110,109;,
  4;115,116,111,110;,
  4;116,117,112,111;,
  4;117,118,113,112;,
  3;21,23,22;,
  3;21,24,23;,
  3;21,25,24;,
  3;21,26,25;,
  3;21,27,26;,
  3;21,28,27;,
  3;21,29,28;,
  3;21,22,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,25,33,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,22,30,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,33,41,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,30,38,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,41,49,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,38,46,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,49,57,56;,
  4;49,50,58,57;,
  4;50,51,59,58;,
  4;51,52,60,59;,
  4;52,53,61,60;,
  4;53,46,54,61;,
  4;54,55,63,62;,
  4;55,56,64,63;,
  4;56,57,65,64;,
  4;57,58,66,65;,
  4;58,59,67,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,54,62,69;,
  4;62,63,71,70;,
  4;63,64,72,71;,
  4;64,65,73,72;,
  4;65,66,74,73;,
  4;66,67,75,74;,
  4;67,68,76,75;,
  4;68,69,77,76;,
  4;69,62,70,77;,
  3;70,71,78;,
  3;71,72,78;,
  3;72,73,78;,
  3;73,74,78;,
  3;74,75,78;,
  3;75,76,78;,
  3;76,77,78;,
  3;77,70,78;,
  4;80,81,81,80;,
  4;81,82,82,81;,
  4;82,83,83,82;,
  4;83,84,84,83;,
  4;84,85,85,84;,
  4;85,86,86,85;,
  4;86,87,87,86;,
  4;87,88,88,87;,
  4;88,89,89,88;,
  4;89,90,90,89;,
  4;90,91,91,90;,
  4;91,92,92,91;,
  4;92,93,93,92;,
  4;93,80,80,93;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;94,94,94;,
  3;94,94,94;,
  3;94,94,94;,
  3;94,94,94;,
  3;94,94,94;,
  3;94,94,94;,
  3;94,94,94;,
  3;94,94,94;,
  3;94,94,94;,
  3;94,94,94;,
  3;94,94,94;,
  3;94,94,94;,
  3;94,94,94;,
  3;94,94,94;,
  4;95,16,20,96;,
  4;95,96,15,11;,
  4;119,118,117,97;,
  4;119,97,120,121;,
  4;96,20,19,98;,
  4;96,98,14,15;,
  4;97,117,116,99;,
  4;97,99,122,120;,
  4;98,19,18,100;,
  4;98,100,13,14;,
  4;99,116,115,101;,
  4;99,101,123,122;,
  4;100,18,17,102;,
  4;100,102,12,13;,
  4;101,115,114,124;,
  4;101,124,125,123;;
 }
 MeshTextureCoords {
  169;
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.500000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.625000;0.250000;,
  0.500000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.625000;0.375000;,
  0.500000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.000000;,
  0.071430;0.000000;,
  0.071430;1.000000;,
  0.000000;1.000000;,
  0.142860;0.000000;,
  0.142860;1.000000;,
  0.214290;0.000000;,
  0.214290;1.000000;,
  0.285710;0.000000;,
  0.285710;1.000000;,
  0.357140;0.000000;,
  0.357140;1.000000;,
  0.428570;0.000000;,
  0.428570;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.571430;0.000000;,
  0.571430;1.000000;,
  0.642860;0.000000;,
  0.642860;1.000000;,
  0.714290;0.000000;,
  0.714290;1.000000;,
  0.785710;0.000000;,
  0.785710;1.000000;,
  0.857140;0.000000;,
  0.857140;1.000000;,
  0.928570;0.000000;,
  0.928570;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.035710;0.000000;,
  0.107140;0.000000;,
  0.178570;0.000000;,
  0.250000;0.000000;,
  0.321430;0.000000;,
  0.392860;0.000000;,
  0.464290;0.000000;,
  0.535710;0.000000;,
  0.607140;0.000000;,
  0.678570;0.000000;,
  0.750000;0.000000;,
  0.821430;0.000000;,
  0.892860;0.000000;,
  0.964290;0.000000;,
  0.035710;1.000000;,
  0.107140;1.000000;,
  0.178570;1.000000;,
  0.250000;1.000000;,
  0.321430;1.000000;,
  0.392860;1.000000;,
  0.464290;1.000000;,
  0.535710;1.000000;,
  0.607140;1.000000;,
  0.678570;1.000000;,
  0.750000;1.000000;,
  0.821430;1.000000;,
  0.892860;1.000000;,
  0.964290;1.000000;,
  1.000000;0.432550;,
  1.000000;0.500000;,
  0.875000;0.500000;,
  0.875000;0.432550;,
  0.875000;0.432550;,
  0.750000;0.500000;,
  0.750000;0.432550;,
  0.750000;0.432550;,
  0.625000;0.500000;,
  0.625000;0.432550;,
  0.625000;0.432550;,
  0.500000;0.500000;,
  0.500000;0.432550;;
 }
}
