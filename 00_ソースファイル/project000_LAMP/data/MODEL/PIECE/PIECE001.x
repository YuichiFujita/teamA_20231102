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
 -2.63383;26.67591;-14.16639;,
 10.05649;47.64568;41.39775;,
 25.63938;0.75166;-5.97592;,
 10.05649;47.64568;41.39775;,
 29.82270;2.74615;8.60376;,
 10.05649;47.64568;41.39775;,
 17.54270;6.05434;30.16481;,
 10.05649;47.64568;41.39775;,
 2.76581;18.27770;36.53736;,
 10.05649;47.64568;41.39775;,
 -16.68669;16.62808;20.13584;,
 10.05649;47.64568;41.39775;,
 -27.99341;7.61017;-5.32126;,
 10.05649;47.64568;41.39775;,
 -27.78154;27.77475;-20.07044;,
 10.05649;47.64568;41.39775;,
 -2.63383;26.67591;-14.16639;,
 -14.17216;10.04134;-6.18233;,
 -2.63383;26.67591;-14.16639;,
 25.63938;0.75166;-5.97592;,
 29.82270;2.74615;8.60376;,
 17.54270;6.05434;30.16481;,
 2.76581;18.27770;36.53736;,
 -16.68669;16.62808;20.13584;,
 -27.99341;7.61017;-5.32126;,
 -27.78154;27.77475;-20.07044;;
 
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
  22;
  0.426335;0.809833;-0.403000;,
  0.729404;0.590787;-0.344878;,
  0.936853;0.331878;0.110290;,
  0.604171;-0.104604;0.789959;,
  -0.324269;-0.610291;0.722769;,
  -0.795050;0.342206;0.500789;,
  -0.839306;0.460538;0.288911;,
  -0.850027;0.316760;0.420853;,
  0.558646;0.739644;-0.375288;,
  0.047965;-0.503302;-0.862778;,
  -0.815525;0.333549;0.472931;,
  0.128307;0.917821;-0.375688;,
  0.044935;-0.457432;-0.888109;,
  -0.084570;-0.382872;-0.919922;,
  -0.172548;-0.959344;0.223354;,
  -0.223839;-0.954948;0.194862;,
  -0.202172;-0.971706;0.122126;,
  -0.351061;-0.913382;0.206129;,
  0.035405;-0.968670;0.245815;,
  0.182903;-0.949458;0.255099;,
  0.053566;-0.589889;-0.805706;,
  0.113762;-0.556992;-0.822689;;
  16;
  3;0,8,1;,
  3;1,8,2;,
  3;2,3,3;,
  3;3,3,4;,
  3;4,10,5;,
  3;5,10,6;,
  3;6,10,7;,
  3;11,8,0;,
  3;9,12,13;,
  3;14,15,16;,
  3;14,16,17;,
  3;14,17,4;,
  3;14,4,18;,
  3;14,18,19;,
  3;9,20,21;,
  3;9,21,12;;
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
