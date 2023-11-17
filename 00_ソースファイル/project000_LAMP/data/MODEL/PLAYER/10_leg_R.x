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
 158;
 -0.04660;-0.06457;-0.74760;,
 4.01840;-0.06457;-12.11230;,
 -0.04660;-0.06457;-12.93530;,
 -0.04660;-0.06457;-0.74760;,
 7.53440;-0.06457;-9.75440;,
 -0.04660;-0.06457;-0.74760;,
 10.02660;-0.06457;-6.18010;,
 -0.04660;-0.06457;-0.74760;,
 11.15830;-0.06457;-1.87210;,
 -0.04660;-0.06457;-0.74760;,
 10.77670;-0.06457;2.58770;,
 -0.04660;-0.06457;-0.74760;,
 8.93340;-0.06457;6.59710;,
 -0.04660;-0.06457;-0.74760;,
 5.87730;-0.06457;9.61460;,
 -0.04660;-0.06457;-0.74760;,
 2.02110;-0.06457;11.23260;,
 -0.04660;-0.06457;-0.74760;,
 -2.11430;-0.06457;11.23260;,
 -0.04660;-0.06457;-0.74760;,
 -5.97050;-0.06457;9.61460;,
 -0.04660;-0.06457;-0.74760;,
 -9.02659;-0.06457;6.59720;,
 -0.04660;-0.06457;-0.74760;,
 -10.86990;-0.06457;2.58770;,
 -0.04660;-0.06457;-0.74760;,
 -11.25150;-0.06457;-1.87210;,
 -0.04660;-0.06457;-0.74760;,
 -10.11980;-0.06457;-6.18010;,
 -0.04660;-0.06457;-0.74760;,
 -7.62759;-0.06457;-9.75440;,
 -0.04660;-0.06457;-0.74760;,
 -4.11160;-0.06457;-12.11230;,
 -0.04660;-0.06457;-0.74760;,
 -0.04660;-0.06457;-12.93530;,
 -0.04660;-21.36878;-0.74760;,
 -0.04660;-21.36878;-12.93530;,
 4.01840;-21.36878;-12.11230;,
 -0.04660;-21.36878;-0.74760;,
 7.53440;-21.36878;-9.75440;,
 -0.04660;-21.36878;-0.74760;,
 10.02660;-21.36878;-6.18010;,
 -0.04660;-21.36878;-0.74760;,
 11.15830;-21.36878;-1.87210;,
 -0.04660;-21.36878;-0.74760;,
 10.77670;-21.36878;2.58770;,
 -0.04660;-21.36878;-0.74760;,
 8.93340;-21.36878;6.59710;,
 -0.04660;-21.36878;-0.74760;,
 5.87730;-21.36878;9.61460;,
 -0.04660;-21.36878;-0.74760;,
 2.02110;-21.36878;11.23260;,
 -0.04660;-21.36878;-0.74760;,
 -2.11430;-21.36878;11.23260;,
 -0.04660;-21.36878;-0.74760;,
 -5.97050;-21.36878;9.61460;,
 -0.04660;-21.36878;-0.74760;,
 -9.02659;-21.36878;6.59720;,
 -0.04660;-21.36878;-0.74760;,
 -10.86990;-21.36878;2.58770;,
 -0.04660;-21.36878;-0.74760;,
 -11.25150;-21.36878;-1.87210;,
 -0.04660;-21.36878;-0.74760;,
 -10.11980;-21.36878;-6.18010;,
 -0.04660;-21.36878;-0.74760;,
 -7.62759;-21.36878;-9.75440;,
 -0.04660;-21.36878;-0.74760;,
 -4.11160;-21.36878;-12.11230;,
 -0.04660;-21.36878;-0.74760;,
 -0.04660;-21.36878;-12.93530;,
 -7.62759;-12.82237;-9.75440;,
 -4.11160;-12.82237;-12.11230;,
 -10.11980;-12.82237;-6.18010;,
 -0.04660;-12.82237;-12.93530;,
 -11.25150;-12.82237;-1.87210;,
 -0.04660;-12.82237;-12.93530;,
 4.01840;-12.82237;-12.11230;,
 -10.86990;-12.82237;2.58770;,
 7.53440;-12.82237;-9.75440;,
 -9.02659;-12.82237;6.59720;,
 10.02660;-12.82237;-6.18010;,
 -5.97050;-12.82237;9.61460;,
 11.15830;-12.82237;-1.87210;,
 -2.11430;-12.82237;11.23260;,
 10.77670;-12.82237;2.58770;,
 2.02110;-12.82237;11.23260;,
 8.93340;-12.82237;6.59710;,
 5.87730;-12.82237;9.61460;,
 -0.12380;-21.46538;-29.93360;,
 4.96410;-21.46538;-28.26260;,
 4.96410;-31.49747;-28.26260;,
 -0.12380;-31.49747;-29.93360;,
 9.36480;-21.46538;-23.47520;,
 9.36480;-31.49747;-23.47520;,
 12.48400;-21.46538;-16.21790;,
 12.48400;-31.49747;-16.21790;,
 13.90060;-21.46538;-7.47100;,
 13.90060;-31.49747;-7.47100;,
 13.42290;-21.46538;1.58430;,
 13.42290;-31.49747;1.58430;,
 11.11580;-21.46538;9.72500;,
 11.11580;-31.49747;9.72500;,
 7.29070;-21.46538;15.85160;,
 7.29070;-31.49747;15.85160;,
 2.46420;-21.46538;19.13680;,
 2.46420;-31.49747;19.13680;,
 -2.71180;-21.46538;19.13680;,
 -2.71180;-31.49747;19.13680;,
 -7.53820;-21.46538;15.85160;,
 -7.53820;-31.49747;15.85160;,
 -11.36340;-21.46538;9.72500;,
 -11.36340;-31.49747;9.72500;,
 -13.67050;-21.46538;1.58430;,
 -13.67050;-31.49747;1.58430;,
 -14.14820;-21.46538;-7.47100;,
 -14.14820;-31.49747;-7.47100;,
 -12.73160;-21.46538;-16.21790;,
 -12.73160;-31.49747;-16.21790;,
 -9.61230;-21.46538;-23.47520;,
 -9.61230;-31.49747;-23.47520;,
 -5.21170;-21.46538;-28.26260;,
 -5.21170;-31.49747;-28.26260;,
 -0.12380;-21.46538;-29.93360;,
 -0.12380;-31.49747;-29.93360;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-21.46538;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;,
 -0.12380;-31.49747;-5.18770;;
 
 119;
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
 3;27,28,26;,
 3;29,30,28;,
 3;31,32,30;,
 3;33,34,32;,
 3;35,36,37;,
 3;38,37,39;,
 3;40,39,41;,
 3;42,41,43;,
 3;44,43,45;,
 3;46,45,47;,
 3;48,47,49;,
 3;50,49,51;,
 3;52,51,53;,
 3;54,53,55;,
 3;56,55,57;,
 3;58,57,59;,
 3;60,59,61;,
 3;62,61,63;,
 3;64,63,65;,
 3;66,65,67;,
 3;68,67,69;,
 4;70,30,32,71;,
 4;70,71,67,65;,
 4;70,65,63,72;,
 4;70,72,28,30;,
 4;71,32,34,73;,
 4;71,73,69,67;,
 4;72,63,61,74;,
 4;72,74,26,28;,
 4;75,2,1,76;,
 4;75,76,37,36;,
 4;74,61,59,77;,
 4;74,77,24,26;,
 4;76,1,4,78;,
 4;76,78,39,37;,
 4;77,59,57,79;,
 4;77,79,22,24;,
 4;78,4,6,80;,
 4;78,80,41,39;,
 4;79,57,55,81;,
 4;79,81,20,22;,
 4;80,6,8,82;,
 4;80,82,43,41;,
 4;81,55,53,83;,
 4;81,83,18,20;,
 4;82,8,10,84;,
 4;82,84,45,43;,
 4;83,53,51,85;,
 4;83,85,16,18;,
 4;84,10,12,86;,
 4;84,86,47,45;,
 4;85,51,49,87;,
 4;85,87,14,16;,
 4;86,12,14,87;,
 4;86,87,49,47;,
 4;88,89,90,91;,
 4;89,92,93,90;,
 4;92,94,95,93;,
 4;94,96,97,95;,
 4;96,98,99,97;,
 4;98,100,101,99;,
 4;100,102,103,101;,
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
 3;124,89,88;,
 3;125,92,89;,
 3;126,94,92;,
 3;127,96,94;,
 3;128,98,96;,
 3;129,100,98;,
 3;130,102,100;,
 3;131,104,102;,
 3;132,106,104;,
 3;133,108,106;,
 3;134,110,108;,
 3;135,112,110;,
 3;136,114,112;,
 3;137,116,114;,
 3;138,118,116;,
 3;139,120,118;,
 3;140,122,120;,
 3;141,91,90;,
 3;142,90,93;,
 3;143,93,95;,
 3;144,95,97;,
 3;145,97,99;,
 3;146,99,101;,
 3;147,101,103;,
 3;148,103,105;,
 3;149,105,107;,
 3;150,107,109;,
 3;151,109,111;,
 3;152,111,113;,
 3;153,113,115;,
 3;154,115,117;,
 3;155,117,119;,
 3;156,119,121;,
 3;157,121,123;;
 
 MeshMaterialList {
  4;
  119;
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
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
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
  56;
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  -0.700662;0.000000;-0.713493;,
  -0.385037;0.000000;-0.922901;,
  -0.907758;0.000000;-0.419494;,
  0.000000;0.000000;-1.000000;,
  -0.996324;0.000000;-0.085663;,
  0.385036;0.000000;-0.922901;,
  -0.966867;0.000000;0.255280;,
  0.700662;0.000000;-0.713494;,
  -0.818877;0.000000;0.573969;,
  0.907758;0.000000;-0.419494;,
  -0.554824;0.000000;0.831968;,
  0.996324;0.000000;-0.085663;,
  -0.197334;0.000000;0.980336;,
  0.966866;0.000000;0.255285;,
  0.197333;0.000000;0.980336;,
  0.818879;0.000000;0.573966;,
  0.554830;0.000000;0.831964;,
  -0.000000;0.000000;-1.000000;,
  0.541646;0.000000;-0.840607;,
  0.839392;0.000000;-0.543527;,
  0.960153;0.000000;-0.279473;,
  0.998546;0.000000;-0.053902;,
  0.986511;0.000000;0.163693;,
  0.914250;0.000000;0.405151;,
  0.720933;0.000000;0.693004;,
  0.294387;0.000000;0.955686;,
  -0.294391;0.000000;0.955685;,
  -0.720933;0.000000;0.693005;,
  -0.914247;0.000000;0.405157;,
  -0.986511;0.000000;0.163693;,
  -0.998546;0.000000;-0.053902;,
  -0.960152;0.000000;-0.279479;,
  -0.839392;0.000000;-0.543527;,
  -0.541651;0.000000;-0.840604;,
  -0.385037;0.000000;-0.922901;,
  -0.385037;0.000000;-0.922901;,
  -0.700662;0.000000;-0.713493;,
  -0.907758;0.000000;-0.419494;,
  -0.907758;0.000000;-0.419494;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;-1.000000;,
  -0.996324;0.000000;-0.085663;,
  -0.966867;0.000000;0.255280;,
  0.700662;0.000000;-0.713493;,
  0.700662;0.000000;-0.713494;,
  -0.818877;0.000000;0.573969;,
  -0.818877;0.000000;0.573969;,
  0.907758;0.000000;-0.419494;,
  -0.554824;0.000000;0.831968;,
  0.996324;0.000000;-0.085663;,
  -0.197334;0.000000;0.980336;,
  -0.197334;0.000000;0.980336;,
  0.966866;0.000000;0.255285;,
  0.966866;0.000000;0.255285;;
  119;
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;2,2,36,3;,
  4;2,3,37,38;,
  4;2,38,39,4;,
  4;2,4,40,2;,
  4;3,36,41,5;,
  4;3,5,42,37;,
  4;4,39,43,6;,
  4;4,6,6,40;,
  4;5,41,7,7;,
  4;5,7,7,42;,
  4;6,43,8,8;,
  4;6,8,44,6;,
  4;7,7,45,9;,
  4;7,9,46,7;,
  4;8,8,47,10;,
  4;8,10,48,44;,
  4;9,45,49,11;,
  4;9,11,11,46;,
  4;10,47,12,12;,
  4;10,12,50,48;,
  4;11,49,13,13;,
  4;11,13,51,11;,
  4;12,12,52,14;,
  4;12,14,53,50;,
  4;13,13,54,15;,
  4;13,15,55,51;,
  4;14,52,16,16;,
  4;14,16,16,53;,
  4;15,54,17,17;,
  4;15,17,17,55;,
  4;16,16,18,18;,
  4;16,18,18,16;,
  4;17,17,18,18;,
  4;17,18,18,17;,
  4;19,20,20,19;,
  4;20,21,21,20;,
  4;21,22,22,21;,
  4;22,23,23,22;,
  4;23,24,24,23;,
  4;24,25,25,24;,
  4;25,26,26,25;,
  4;26,27,27,26;,
  4;27,28,28,27;,
  4;28,29,29,28;,
  4;29,30,30,29;,
  4;30,31,31,30;,
  4;31,32,32,31;,
  4;32,33,33,32;,
  4;33,34,34,33;,
  4;34,35,35,34;,
  4;35,19,19,35;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;;
 }
 MeshTextureCoords {
  158;
  0.029410;0.000000;,
  0.058820;0.000000;,
  0.000000;0.000000;,
  0.088240;0.000000;,
  0.117650;0.000000;,
  0.147060;0.000000;,
  0.176470;0.000000;,
  0.205880;0.000000;,
  0.235290;0.000000;,
  0.264710;0.000000;,
  0.294120;0.000000;,
  0.323530;0.000000;,
  0.352940;0.000000;,
  0.382350;0.000000;,
  0.411760;0.000000;,
  0.441180;0.000000;,
  0.470590;0.000000;,
  0.500000;0.000000;,
  0.529410;0.000000;,
  0.558820;0.000000;,
  0.588240;0.000000;,
  0.617650;0.000000;,
  0.647060;0.000000;,
  0.676470;0.000000;,
  0.705880;0.000000;,
  0.735290;0.000000;,
  0.764710;0.000000;,
  0.794120;0.000000;,
  0.823530;0.000000;,
  0.852940;0.000000;,
  0.882350;0.000000;,
  0.911760;0.000000;,
  0.941180;0.000000;,
  0.970590;0.000000;,
  1.000000;0.000000;,
  0.029410;1.000000;,
  0.000000;1.000000;,
  0.058820;1.000000;,
  0.088240;1.000000;,
  0.117650;1.000000;,
  0.147060;1.000000;,
  0.176470;1.000000;,
  0.205880;1.000000;,
  0.235290;1.000000;,
  0.264710;1.000000;,
  0.294120;1.000000;,
  0.323530;1.000000;,
  0.352940;1.000000;,
  0.382350;1.000000;,
  0.411760;1.000000;,
  0.441180;1.000000;,
  0.470590;1.000000;,
  0.500000;1.000000;,
  0.529410;1.000000;,
  0.558820;1.000000;,
  0.588240;1.000000;,
  0.617650;1.000000;,
  0.647060;1.000000;,
  0.676470;1.000000;,
  0.705880;1.000000;,
  0.735290;1.000000;,
  0.764710;1.000000;,
  0.794120;1.000000;,
  0.823530;1.000000;,
  0.852940;1.000000;,
  0.882350;1.000000;,
  0.911760;1.000000;,
  0.941180;1.000000;,
  0.970590;1.000000;,
  1.000000;1.000000;,
  0.882350;0.598840;,
  0.941180;0.598840;,
  0.823530;0.598840;,
  1.000000;0.598840;,
  0.764710;0.598840;,
  0.000000;0.598840;,
  0.058820;0.598840;,
  0.705880;0.598840;,
  0.117650;0.598840;,
  0.647060;0.598840;,
  0.176470;0.598840;,
  0.588240;0.598840;,
  0.235290;0.598840;,
  0.529410;0.598840;,
  0.294120;0.598840;,
  0.470590;0.598840;,
  0.352940;0.598840;,
  0.411760;0.598840;,
  0.000000;0.000000;,
  0.058820;0.000000;,
  0.058820;1.000000;,
  0.000000;1.000000;,
  0.117650;0.000000;,
  0.117650;1.000000;,
  0.176470;0.000000;,
  0.176470;1.000000;,
  0.235290;0.000000;,
  0.235290;1.000000;,
  0.294120;0.000000;,
  0.294120;1.000000;,
  0.352940;0.000000;,
  0.352940;1.000000;,
  0.411760;0.000000;,
  0.411760;1.000000;,
  0.470590;0.000000;,
  0.470590;1.000000;,
  0.529410;0.000000;,
  0.529410;1.000000;,
  0.588240;0.000000;,
  0.588240;1.000000;,
  0.647060;0.000000;,
  0.647060;1.000000;,
  0.705880;0.000000;,
  0.705880;1.000000;,
  0.764710;0.000000;,
  0.764710;1.000000;,
  0.823530;0.000000;,
  0.823530;1.000000;,
  0.882350;0.000000;,
  0.882350;1.000000;,
  0.941180;0.000000;,
  0.941180;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.029410;0.000000;,
  0.088240;0.000000;,
  0.147060;0.000000;,
  0.205880;0.000000;,
  0.264710;0.000000;,
  0.323530;0.000000;,
  0.382350;0.000000;,
  0.441180;0.000000;,
  0.500000;0.000000;,
  0.558820;0.000000;,
  0.617650;0.000000;,
  0.676470;0.000000;,
  0.735290;0.000000;,
  0.794120;0.000000;,
  0.852940;0.000000;,
  0.911760;0.000000;,
  0.970590;0.000000;,
  0.029410;1.000000;,
  0.088240;1.000000;,
  0.147060;1.000000;,
  0.205880;1.000000;,
  0.264710;1.000000;,
  0.323530;1.000000;,
  0.382350;1.000000;,
  0.441180;1.000000;,
  0.500000;1.000000;,
  0.558820;1.000000;,
  0.617650;1.000000;,
  0.676470;1.000000;,
  0.735290;1.000000;,
  0.794120;1.000000;,
  0.852940;1.000000;,
  0.911760;1.000000;,
  0.970590;1.000000;;
 }
}
