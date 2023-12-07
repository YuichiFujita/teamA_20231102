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
 26;
 -1.49621;8.92195;-6.28732;,
 0.82161;17.12792;12.30268;,
 9.26945;0.00597;-5.46805;,
 0.82161;17.12792;12.30268;,
 10.28368;0.65393;1.83462;,
 0.82161;17.12792;12.30268;,
 5.01847;1.98433;13.43957;,
 0.82161;17.12792;12.30268;,
 -0.06366;3.12744;15.63124;,
 0.82161;17.12792;12.30268;,
 -9.48885;3.55405;9.99037;,
 0.82161;17.12792;12.30268;,
 -13.37750;3.51337;1.23505;,
 0.82161;17.12792;12.30268;,
 -10.56964;9.45451;-5.52365;,
 0.82161;17.12792;12.30268;,
 -1.49621;8.92195;-6.28732;,
 -5.88904;0.10212;-2.87093;,
 -1.49621;8.92195;-6.28732;,
 9.26945;0.00597;-5.46805;,
 10.28368;0.65393;1.83462;,
 5.01847;1.98433;13.43957;,
 -0.06366;3.12744;15.63124;,
 -9.48885;3.55405;9.99037;,
 -13.37750;3.51337;1.23505;,
 -10.56964;9.45451;-5.52365;;
 
 16;
 3;0,1,2;,
 3;2,3,4;,
 3;4,5,6;,
 3;6,7,8;,
 3;8,9,10;,
 3;10,11,12;,
 3;12,13,14;,
 3;14,15,16;,
 3;17,18,19;,
 3;17,19,20;,
 3;17,20,21;,
 3;17,21,22;,
 3;17,22,23;,
 3;17,23,24;,
 3;17,24,25;,
 3;17,25,18;;
 
 MeshMaterialList {
  1;
  16;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  21;
  0.329433;0.847910;-0.415358;,
  0.716457;0.640490;-0.276519;,
  0.890415;0.441743;0.109656;,
  0.699704;0.244316;0.671359;,
  -0.038348;0.233424;0.971619;,
  -0.669673;0.402334;0.624231;,
  -0.769483;0.572667;0.282751;,
  -0.759428;0.611525;0.222051;,
  0.677521;0.715726;-0.169415;,
  -0.290846;-0.372859;-0.881127;,
  0.019480;0.913763;-0.405781;,
  -0.130909;-0.300726;-0.944683;,
  -0.163650;-0.284400;-0.944635;,
  -0.127348;-0.979689;0.154896;,
  0.008620;-0.996154;0.087191;,
  0.004646;-0.994906;0.100703;,
  -0.065867;-0.985280;0.157748;,
  -0.150907;-0.966928;0.205616;,
  -0.255010;-0.949380;0.183434;,
  -0.523426;-0.801727;-0.288548;,
  -0.354075;-0.414933;-0.838130;;
  16;
  3;0,8,1;,
  3;1,8,2;,
  3;2,8,3;,
  3;3,4,4;,
  3;4,4,5;,
  3;5,6,6;,
  3;6,6,7;,
  3;10,8,0;,
  3;9,11,12;,
  3;13,14,15;,
  3;13,15,16;,
  3;13,16,17;,
  3;13,17,18;,
  3;13,18,19;,
  3;9,19,20;,
  3;9,20,11;;
 }
 MeshTextureCoords {
  26;
  0.000000;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.187500;0.000000;,
  0.375000;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.312500;0.000000;,
  0.625000;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.437500;0.000000;,
  0.875000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
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
