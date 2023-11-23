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
 388;
 -224.49370;184.98106;-65.84036;,
 224.70557;184.98106;-65.84036;,
 224.70557;1.04176;-65.84036;,
 -224.49370;1.04176;-65.84036;,
 224.70557;184.98106;-65.84036;,
 224.70557;184.98106;65.17234;,
 224.70557;1.04176;65.17234;,
 224.70557;1.04176;-65.84036;,
 224.70557;184.98106;65.17234;,
 -224.49370;184.98106;65.17234;,
 -224.49370;1.04176;65.17234;,
 224.70557;1.04176;65.17234;,
 -224.49370;184.98106;65.17234;,
 -224.49370;184.98106;-65.84036;,
 -224.49370;1.04176;-65.84036;,
 -224.49370;1.04176;65.17234;,
 224.70557;184.98106;-65.84036;,
 -224.49370;184.98106;-65.84036;,
 -224.49370;1.04176;-65.84036;,
 224.70557;1.04176;-65.84036;,
 -224.49656;238.35894;24.81222;,
 -168.36701;238.35894;24.81222;,
 -168.36701;184.42965;24.81222;,
 -224.49656;184.42965;24.81222;,
 -168.36701;238.35894;24.81222;,
 -168.36701;238.35894;66.00173;,
 -168.36701;184.42965;66.00173;,
 -168.36701;184.42965;24.81222;,
 -168.36701;238.35894;66.00173;,
 -224.49656;238.35894;66.00173;,
 -224.49656;184.42965;66.00173;,
 -168.36701;184.42965;66.00173;,
 -224.49656;238.35894;66.00173;,
 -224.49656;238.35894;24.81222;,
 -224.49656;184.42965;24.81222;,
 -224.49656;184.42965;66.00173;,
 -168.36701;238.35894;24.81222;,
 -224.49656;238.35894;24.81222;,
 -224.49656;184.42965;24.81222;,
 -168.36701;184.42965;24.81222;,
 -93.99592;238.35894;24.81222;,
 -37.86641;238.35894;24.81222;,
 -37.86641;184.42965;24.81222;,
 -93.99592;184.42965;24.81222;,
 -37.86641;238.35894;24.81222;,
 -37.86641;238.35894;66.00173;,
 -37.86641;184.42965;66.00173;,
 -37.86641;184.42965;24.81222;,
 -37.86641;238.35894;66.00173;,
 -93.99592;238.35894;66.00173;,
 -93.99592;184.42965;66.00173;,
 -37.86641;184.42965;66.00173;,
 -93.99592;238.35894;66.00173;,
 -93.99592;238.35894;24.81222;,
 -93.99592;184.42965;24.81222;,
 -93.99592;184.42965;66.00173;,
 -37.86641;238.35894;24.81222;,
 -93.99592;238.35894;24.81222;,
 -93.99592;184.42965;24.81222;,
 -37.86641;184.42965;24.81222;,
 43.64321;238.35894;24.81222;,
 99.77269;238.35894;24.81222;,
 99.77269;184.42965;24.81222;,
 43.64321;184.42965;24.81222;,
 99.77269;238.35894;24.81222;,
 99.77269;238.35894;66.00173;,
 99.77269;184.42965;66.00173;,
 99.77269;184.42965;24.81222;,
 99.77269;238.35894;66.00173;,
 43.64321;238.35894;66.00173;,
 43.64321;184.42965;66.00173;,
 99.77269;184.42965;66.00173;,
 43.64321;238.35894;66.00173;,
 43.64321;238.35894;24.81222;,
 43.64321;184.42965;24.81222;,
 43.64321;184.42965;66.00173;,
 99.77269;238.35894;24.81222;,
 43.64321;238.35894;24.81222;,
 43.64321;184.42965;24.81222;,
 99.77269;184.42965;24.81222;,
 167.89770;238.35894;24.81222;,
 224.02713;238.35894;24.81222;,
 224.02713;184.42965;24.81222;,
 167.89770;184.42965;24.81222;,
 224.02713;238.35894;24.81222;,
 224.02713;238.35894;66.00173;,
 224.02713;184.42965;66.00173;,
 224.02713;184.42965;24.81222;,
 224.02713;238.35894;66.00173;,
 167.89770;238.35894;66.00173;,
 167.89770;184.42965;66.00173;,
 224.02713;184.42965;66.00173;,
 167.89770;238.35894;66.00173;,
 167.89770;238.35894;24.81222;,
 167.89770;184.42965;24.81222;,
 167.89770;184.42965;66.00173;,
 224.02713;238.35894;24.81222;,
 167.89770;238.35894;24.81222;,
 167.89770;184.42965;24.81222;,
 224.02713;184.42965;24.81222;,
 -66.04741;237.30718;8.33841;,
 -66.04741;277.05162;45.40698;,
 -40.09616;237.30718;19.19551;,
 -66.04741;277.05162;45.40698;,
 -29.34671;237.30718;45.40698;,
 -66.04741;277.05162;45.40698;,
 -40.09616;237.30718;71.61846;,
 -66.04741;277.05162;45.40698;,
 -66.04747;237.30718;82.47557;,
 -66.04741;277.05162;45.40698;,
 -91.99861;237.30718;71.61846;,
 -66.04741;277.05162;45.40698;,
 -102.74818;237.30718;45.40698;,
 -66.04741;277.05162;45.40698;,
 -91.99861;237.30718;19.19551;,
 -66.04741;277.05162;45.40698;,
 -66.04741;237.30718;8.33841;,
 -66.04741;237.30718;45.40698;,
 -66.04741;237.30718;8.33841;,
 -40.09616;237.30718;19.19551;,
 -29.34671;237.30718;45.40698;,
 -40.09616;237.30718;71.61846;,
 -66.04747;237.30718;82.47557;,
 -91.99861;237.30718;71.61846;,
 -102.74818;237.30718;45.40698;,
 -91.99861;237.30718;19.19551;,
 72.31329;237.30718;8.33841;,
 72.31329;277.05162;45.40698;,
 98.26462;237.30718;19.19551;,
 72.31329;277.05162;45.40698;,
 109.01403;237.30718;45.40698;,
 72.31329;277.05162;45.40698;,
 98.26462;237.30718;71.61846;,
 72.31329;277.05162;45.40698;,
 72.31329;237.30718;82.47557;,
 72.31329;277.05162;45.40698;,
 46.36214;237.30718;71.61846;,
 72.31329;277.05162;45.40698;,
 35.61260;237.30718;45.40698;,
 72.31329;277.05162;45.40698;,
 46.36214;237.30718;19.19551;,
 72.31329;277.05162;45.40698;,
 72.31329;237.30718;8.33841;,
 72.31329;237.30718;45.40698;,
 72.31329;237.30718;8.33841;,
 98.26462;237.30718;19.19551;,
 109.01403;237.30718;45.40698;,
 98.26462;237.30718;71.61846;,
 72.31329;237.30718;82.47557;,
 46.36214;237.30718;71.61846;,
 35.61260;237.30718;45.40698;,
 46.36214;237.30718;19.19551;,
 -198.25846;237.30718;8.33841;,
 -198.25846;277.05162;45.40698;,
 -172.30730;237.30718;19.19551;,
 -198.25846;277.05162;45.40698;,
 -161.55783;237.30718;45.40698;,
 -198.25846;277.05162;45.40698;,
 -172.30730;237.30718;71.61846;,
 -198.25846;277.05162;45.40698;,
 -198.25846;237.30718;82.47557;,
 -198.25846;277.05162;45.40698;,
 -224.20974;237.30718;71.61846;,
 -198.25846;277.05162;45.40698;,
 -234.95926;237.30718;45.40698;,
 -198.25846;277.05162;45.40698;,
 -224.20974;237.30718;19.19551;,
 -198.25846;277.05162;45.40698;,
 -198.25846;237.30718;8.33841;,
 -198.25846;237.30718;45.40698;,
 -198.25846;237.30718;8.33841;,
 -172.30730;237.30718;19.19551;,
 -161.55783;237.30718;45.40698;,
 -172.30730;237.30718;71.61846;,
 -198.25846;237.30718;82.47557;,
 -224.20974;237.30718;71.61846;,
 -234.95926;237.30718;45.40698;,
 -224.20974;237.30718;19.19551;,
 198.19811;237.30718;8.33841;,
 198.19811;277.05162;45.40698;,
 224.14944;237.30718;19.19551;,
 198.19811;277.05162;45.40698;,
 234.89881;237.30718;45.40698;,
 198.19811;277.05162;45.40698;,
 224.14944;237.30718;71.61846;,
 198.19811;277.05162;45.40698;,
 198.19811;237.30718;82.47557;,
 198.19811;277.05162;45.40698;,
 172.24699;237.30718;71.61846;,
 198.19811;277.05162;45.40698;,
 161.49743;237.30718;45.40698;,
 198.19811;277.05162;45.40698;,
 172.24699;237.30718;19.19551;,
 198.19811;277.05162;45.40698;,
 198.19811;237.30718;8.33841;,
 198.19811;237.30718;45.40698;,
 198.19811;237.30718;8.33841;,
 224.14944;237.30718;19.19551;,
 234.89881;237.30718;45.40698;,
 224.14944;237.30718;71.61846;,
 198.19811;237.30718;82.47557;,
 172.24699;237.30718;71.61846;,
 161.49743;237.30718;45.40698;,
 172.24699;237.30718;19.19551;,
 -224.49656;238.35894;-65.63969;,
 -168.36701;238.35894;-65.63969;,
 -168.36701;184.42965;-65.63969;,
 -224.49656;184.42965;-65.63969;,
 -168.36701;238.35894;-65.63969;,
 -168.36701;238.35894;-24.45021;,
 -168.36701;184.42965;-24.45021;,
 -168.36701;184.42965;-65.63969;,
 -168.36701;238.35894;-24.45021;,
 -224.49656;238.35894;-24.45021;,
 -224.49656;184.42965;-24.45021;,
 -168.36701;184.42965;-24.45021;,
 -224.49656;238.35894;-24.45021;,
 -224.49656;238.35894;-65.63969;,
 -224.49656;184.42965;-65.63969;,
 -224.49656;184.42965;-24.45021;,
 -168.36701;238.35894;-65.63969;,
 -224.49656;238.35894;-65.63969;,
 -224.49656;184.42965;-65.63969;,
 -168.36701;184.42965;-65.63969;,
 -93.99592;238.35894;-65.63969;,
 -37.86641;238.35894;-65.63969;,
 -37.86641;184.42965;-65.63969;,
 -93.99592;184.42965;-65.63969;,
 -37.86641;238.35894;-65.63969;,
 -37.86641;238.35894;-24.45021;,
 -37.86641;184.42965;-24.45021;,
 -37.86641;184.42965;-65.63969;,
 -37.86641;238.35894;-24.45021;,
 -93.99592;238.35894;-24.45021;,
 -93.99592;184.42965;-24.45021;,
 -37.86641;184.42965;-24.45021;,
 -93.99592;238.35894;-24.45021;,
 -93.99592;238.35894;-65.63969;,
 -93.99592;184.42965;-65.63969;,
 -93.99592;184.42965;-24.45021;,
 -37.86641;238.35894;-65.63969;,
 -93.99592;238.35894;-65.63969;,
 -93.99592;184.42965;-65.63969;,
 -37.86641;184.42965;-65.63969;,
 43.64321;238.35894;-65.63969;,
 99.77269;238.35894;-65.63969;,
 99.77269;184.42965;-65.63969;,
 43.64321;184.42965;-65.63969;,
 99.77269;238.35894;-65.63969;,
 99.77269;238.35894;-24.45021;,
 99.77269;184.42965;-24.45021;,
 99.77269;184.42965;-65.63969;,
 99.77269;238.35894;-24.45021;,
 43.64321;238.35894;-24.45021;,
 43.64321;184.42965;-24.45021;,
 99.77269;184.42965;-24.45021;,
 43.64321;238.35894;-24.45021;,
 43.64321;238.35894;-65.63969;,
 43.64321;184.42965;-65.63969;,
 43.64321;184.42965;-24.45021;,
 99.77269;238.35894;-65.63969;,
 43.64321;238.35894;-65.63969;,
 43.64321;184.42965;-65.63969;,
 99.77269;184.42965;-65.63969;,
 167.89770;238.35894;-65.63969;,
 224.02713;238.35894;-65.63969;,
 224.02713;184.42965;-65.63969;,
 167.89770;184.42965;-65.63969;,
 224.02713;238.35894;-65.63969;,
 224.02713;238.35894;-24.45021;,
 224.02713;184.42965;-24.45021;,
 224.02713;184.42965;-65.63969;,
 224.02713;238.35894;-24.45021;,
 167.89770;238.35894;-24.45021;,
 167.89770;184.42965;-24.45021;,
 224.02713;184.42965;-24.45021;,
 167.89770;238.35894;-24.45021;,
 167.89770;238.35894;-65.63969;,
 167.89770;184.42965;-65.63969;,
 167.89770;184.42965;-24.45021;,
 224.02713;238.35894;-65.63969;,
 167.89770;238.35894;-65.63969;,
 167.89770;184.42965;-65.63969;,
 224.02713;184.42965;-65.63969;,
 -66.04741;237.30718;-82.28484;,
 -66.04741;277.05162;-45.21626;,
 -40.09616;237.30718;-71.42773;,
 -66.04741;277.05162;-45.21626;,
 -29.34671;237.30718;-45.21626;,
 -66.04741;277.05162;-45.21626;,
 -40.09616;237.30718;-19.00484;,
 -66.04741;277.05162;-45.21626;,
 -66.04747;237.30718;-8.14773;,
 -66.04741;277.05162;-45.21626;,
 -91.99861;237.30718;-19.00484;,
 -66.04741;277.05162;-45.21626;,
 -102.74818;237.30718;-45.21626;,
 -66.04741;277.05162;-45.21626;,
 -91.99861;237.30718;-71.42773;,
 -66.04741;277.05162;-45.21626;,
 -66.04741;237.30718;-82.28484;,
 -66.04741;237.30718;-45.21626;,
 -66.04741;237.30718;-82.28484;,
 -40.09616;237.30718;-71.42773;,
 -29.34671;237.30718;-45.21626;,
 -40.09616;237.30718;-19.00484;,
 -66.04747;237.30718;-8.14773;,
 -91.99861;237.30718;-19.00484;,
 -102.74818;237.30718;-45.21626;,
 -91.99861;237.30718;-71.42773;,
 72.31329;237.30718;-82.28484;,
 72.31329;277.05162;-45.21626;,
 98.26462;237.30718;-71.42773;,
 72.31329;277.05162;-45.21626;,
 109.01403;237.30718;-45.21626;,
 72.31329;277.05162;-45.21626;,
 98.26462;237.30718;-19.00484;,
 72.31329;277.05162;-45.21626;,
 72.31329;237.30718;-8.14773;,
 72.31329;277.05162;-45.21626;,
 46.36214;237.30718;-19.00484;,
 72.31329;277.05162;-45.21626;,
 35.61260;237.30718;-45.21626;,
 72.31329;277.05162;-45.21626;,
 46.36214;237.30718;-71.42773;,
 72.31329;277.05162;-45.21626;,
 72.31329;237.30718;-82.28484;,
 72.31329;237.30718;-45.21626;,
 72.31329;237.30718;-82.28484;,
 98.26462;237.30718;-71.42773;,
 109.01403;237.30718;-45.21626;,
 98.26462;237.30718;-19.00484;,
 72.31329;237.30718;-8.14773;,
 46.36214;237.30718;-19.00484;,
 35.61260;237.30718;-45.21626;,
 46.36214;237.30718;-71.42773;,
 -198.25846;237.30718;-82.28484;,
 -198.25846;277.05162;-45.21626;,
 -172.30730;237.30718;-71.42773;,
 -198.25846;277.05162;-45.21626;,
 -161.55783;237.30718;-45.21626;,
 -198.25846;277.05162;-45.21626;,
 -172.30730;237.30718;-19.00484;,
 -198.25846;277.05162;-45.21626;,
 -198.25846;237.30718;-8.14773;,
 -198.25846;277.05162;-45.21626;,
 -224.20974;237.30718;-19.00484;,
 -198.25846;277.05162;-45.21626;,
 -234.95926;237.30718;-45.21626;,
 -198.25846;277.05162;-45.21626;,
 -224.20974;237.30718;-71.42773;,
 -198.25846;277.05162;-45.21626;,
 -198.25846;237.30718;-82.28484;,
 -198.25846;237.30718;-45.21626;,
 -198.25846;237.30718;-82.28484;,
 -172.30730;237.30718;-71.42773;,
 -161.55783;237.30718;-45.21626;,
 -172.30730;237.30718;-19.00484;,
 -198.25846;237.30718;-8.14773;,
 -224.20974;237.30718;-19.00484;,
 -234.95926;237.30718;-45.21626;,
 -224.20974;237.30718;-71.42773;,
 198.19811;237.30718;-82.28484;,
 198.19811;277.05162;-45.21626;,
 224.14944;237.30718;-71.42773;,
 198.19811;277.05162;-45.21626;,
 234.89881;237.30718;-45.21626;,
 198.19811;277.05162;-45.21626;,
 224.14944;237.30718;-19.00484;,
 198.19811;277.05162;-45.21626;,
 198.19811;237.30718;-8.14773;,
 198.19811;277.05162;-45.21626;,
 172.24699;237.30718;-19.00484;,
 198.19811;277.05162;-45.21626;,
 161.49743;237.30718;-45.21626;,
 198.19811;277.05162;-45.21626;,
 172.24699;237.30718;-71.42773;,
 198.19811;277.05162;-45.21626;,
 198.19811;237.30718;-82.28484;,
 198.19811;237.30718;-45.21626;,
 198.19811;237.30718;-82.28484;,
 224.14944;237.30718;-71.42773;,
 234.89881;237.30718;-45.21626;,
 224.14944;237.30718;-19.00484;,
 198.19811;237.30718;-8.14773;,
 172.24699;237.30718;-19.00484;,
 161.49743;237.30718;-45.21626;,
 172.24699;237.30718;-71.42773;;
 
 182;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;12,5,16,17;,
 4;18,19,6,15;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;32,25,36,37;,
 4;38,39,26,35;,
 4;40,41,42,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;52,53,54,55;,
 4;52,45,56,57;,
 4;58,59,46,55;,
 4;60,61,62,63;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;72,73,74,75;,
 4;72,65,76,77;,
 4;78,79,66,75;,
 4;80,81,82,83;,
 4;84,85,86,87;,
 4;88,89,90,91;,
 4;92,93,94,95;,
 4;92,85,96,97;,
 4;98,99,86,95;,
 3;100,101,102;,
 3;102,103,104;,
 3;104,105,106;,
 3;106,107,108;,
 3;108,109,110;,
 3;110,111,112;,
 3;112,113,114;,
 3;114,115,116;,
 3;117,118,119;,
 3;117,119,120;,
 3;117,120,121;,
 3;117,121,122;,
 3;117,122,123;,
 3;117,123,124;,
 3;117,124,125;,
 3;117,125,118;,
 3;126,127,128;,
 3;128,129,130;,
 3;130,131,132;,
 3;132,133,134;,
 3;134,135,136;,
 3;136,137,138;,
 3;138,139,140;,
 3;140,141,142;,
 3;143,144,145;,
 3;143,145,146;,
 3;143,146,147;,
 3;143,147,148;,
 3;143,148,149;,
 3;143,149,150;,
 3;143,150,151;,
 3;143,151,144;,
 3;152,153,154;,
 3;154,155,156;,
 3;156,157,158;,
 3;158,159,160;,
 3;160,161,162;,
 3;162,163,164;,
 3;164,165,166;,
 3;166,167,168;,
 3;169,170,171;,
 3;169,171,172;,
 3;169,172,173;,
 3;169,173,174;,
 3;169,174,175;,
 3;169,175,176;,
 3;169,176,177;,
 3;169,177,170;,
 3;178,179,180;,
 3;180,181,182;,
 3;182,183,184;,
 3;184,185,186;,
 3;186,187,188;,
 3;188,189,190;,
 3;190,191,192;,
 3;192,193,194;,
 3;195,196,197;,
 3;195,197,198;,
 3;195,198,199;,
 3;195,199,200;,
 3;195,200,201;,
 3;195,201,202;,
 3;195,202,203;,
 3;195,203,196;,
 4;204,205,206,207;,
 4;208,209,210,211;,
 4;212,213,214,215;,
 4;216,217,218,219;,
 4;216,209,220,221;,
 4;222,223,210,219;,
 4;224,225,226,227;,
 4;228,229,230,231;,
 4;232,233,234,235;,
 4;236,237,238,239;,
 4;236,229,240,241;,
 4;242,243,230,239;,
 4;244,245,246,247;,
 4;248,249,250,251;,
 4;252,253,254,255;,
 4;256,257,258,259;,
 4;256,249,260,261;,
 4;262,263,250,259;,
 4;264,265,266,267;,
 4;268,269,270,271;,
 4;272,273,274,275;,
 4;276,277,278,279;,
 4;276,269,280,281;,
 4;282,283,270,279;,
 3;284,285,286;,
 3;286,287,288;,
 3;288,289,290;,
 3;290,291,292;,
 3;292,293,294;,
 3;294,295,296;,
 3;296,297,298;,
 3;298,299,300;,
 3;301,302,303;,
 3;301,303,304;,
 3;301,304,305;,
 3;301,305,306;,
 3;301,306,307;,
 3;301,307,308;,
 3;301,308,309;,
 3;301,309,302;,
 3;310,311,312;,
 3;312,313,314;,
 3;314,315,316;,
 3;316,317,318;,
 3;318,319,320;,
 3;320,321,322;,
 3;322,323,324;,
 3;324,325,326;,
 3;327,328,329;,
 3;327,329,330;,
 3;327,330,331;,
 3;327,331,332;,
 3;327,332,333;,
 3;327,333,334;,
 3;327,334,335;,
 3;327,335,328;,
 3;336,337,338;,
 3;338,339,340;,
 3;340,341,342;,
 3;342,343,344;,
 3;344,345,346;,
 3;346,347,348;,
 3;348,349,350;,
 3;350,351,352;,
 3;353,354,355;,
 3;353,355,356;,
 3;353,356,357;,
 3;353,357,358;,
 3;353,358,359;,
 3;353,359,360;,
 3;353,360,361;,
 3;353,361,354;,
 3;362,363,364;,
 3;364,365,366;,
 3;366,367,368;,
 3;368,369,370;,
 3;370,371,372;,
 3;372,373,374;,
 3;374,375,376;,
 3;376,377,378;,
 3;379,380,381;,
 3;379,381,382;,
 3;379,382,383;,
 3;379,383,384;,
 3;379,384,385;,
 3;379,385,386;,
 3;379,386,387;,
 3;379,387,380;;
 
 MeshMaterialList {
  3;
  182;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\MODEL\\TEXTURE\\Rock-Wall002.png";
   }
  }
  Material {
   0.357600;0.357600;0.357600;1.000000;;
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
 }
 MeshNormals {
  100;
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;0.682060;-0.731296;,
  0.520450;0.680233;-0.516154;,
  0.734679;0.678415;-0.000000;,
  0.520450;0.680233;0.516154;,
  -0.000001;0.682060;0.731296;,
  -0.520450;0.680233;0.516155;,
  -0.734678;0.678416;-0.000000;,
  -0.520449;0.680233;-0.516155;,
  0.634010;0.727990;-0.260886;,
  0.000000;-1.000000;-0.000000;,
  -0.000001;0.682060;-0.731296;,
  0.520449;0.680234;-0.516154;,
  0.734678;0.678416;-0.000000;,
  0.520450;0.680234;0.516153;,
  -0.000001;0.682060;0.731296;,
  -0.520450;0.680232;0.516155;,
  -0.734679;0.678415;-0.000000;,
  -0.520450;0.680232;-0.516155;,
  0.634010;0.727991;-0.260886;,
  0.000001;0.682060;-0.731296;,
  0.520450;0.680232;-0.516154;,
  0.734679;0.678414;-0.000000;,
  0.520451;0.680233;0.516154;,
  0.000001;0.682060;0.731296;,
  -0.520449;0.680233;0.516154;,
  -0.734678;0.678416;-0.000000;,
  -0.520449;0.680233;-0.516155;,
  0.634011;0.727990;-0.260886;,
  -0.000001;0.682060;-0.731296;,
  0.520450;0.680234;-0.516153;,
  0.734679;0.678415;-0.000000;,
  0.520450;0.680234;0.516153;,
  -0.000001;0.682060;0.731296;,
  -0.520450;0.680232;0.516155;,
  -0.734679;0.678415;-0.000000;,
  -0.520450;0.680232;-0.516155;,
  0.634010;0.727990;-0.260886;,
  -0.262903;0.730286;0.630527;,
  -0.262904;0.730286;0.630527;,
  -0.262902;0.730286;0.630527;,
  -0.262904;0.730286;0.630527;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;0.682060;-0.731296;,
  0.520450;0.680233;-0.516154;,
  0.734679;0.678415;0.000000;,
  0.520450;0.680233;0.516155;,
  -0.000001;0.682060;0.731296;,
  -0.520450;0.680232;0.516155;,
  -0.734678;0.678416;0.000000;,
  -0.520449;0.680233;-0.516155;,
  0.634010;0.727990;-0.260886;,
  0.000000;-1.000000;-0.000000;,
  -0.000001;0.682060;-0.731296;,
  0.520450;0.680234;-0.516154;,
  0.734678;0.678415;0.000000;,
  0.520450;0.680233;0.516154;,
  -0.000001;0.682060;0.731296;,
  -0.520450;0.680232;0.516155;,
  -0.734679;0.678415;0.000000;,
  -0.520450;0.680232;-0.516155;,
  0.634010;0.727991;-0.260886;,
  0.000001;0.682060;-0.731296;,
  0.520451;0.680232;-0.516154;,
  0.734679;0.678414;0.000000;,
  0.520451;0.680232;0.516155;,
  0.000001;0.682060;0.731296;,
  -0.520449;0.680233;0.516155;,
  -0.734678;0.678416;0.000000;,
  -0.520449;0.680233;-0.516154;,
  0.634011;0.727990;-0.260886;,
  -0.000001;0.682060;-0.731296;,
  0.520450;0.680234;-0.516153;,
  0.734679;0.678415;0.000000;,
  0.520450;0.680233;0.516154;,
  -0.000001;0.682060;0.731296;,
  -0.520450;0.680232;0.516155;,
  -0.734679;0.678415;0.000000;,
  -0.520450;0.680232;-0.516155;,
  0.634010;0.727990;-0.260886;,
  -0.262903;0.730285;0.630528;,
  -0.262904;0.730285;0.630528;,
  -0.262902;0.730286;0.630527;,
  -0.262904;0.730285;0.630528;;
  182;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  3;12,20,13;,
  3;13,20,14;,
  3;14,20,15;,
  3;15,49,16;,
  3;16,49,17;,
  3;17,49,18;,
  3;18,19,19;,
  3;19,19,12;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;22,30,23;,
  3;23,30,24;,
  3;24,30,25;,
  3;25,50,26;,
  3;26,50,27;,
  3;27,50,28;,
  3;28,29,29;,
  3;29,29,22;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;31,39,32;,
  3;32,39,33;,
  3;33,39,34;,
  3;34,51,35;,
  3;35,51,36;,
  3;36,51,37;,
  3;37,38,38;,
  3;38,38,31;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;40,48,41;,
  3;41,48,42;,
  3;42,48,43;,
  3;43,52,44;,
  3;44,52,45;,
  3;45,52,46;,
  3;46,47,47;,
  3;47,47,40;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  4;53,53,53,53;,
  4;54,54,54,54;,
  4;55,55,55,55;,
  4;56,56,56,56;,
  4;57,57,57,57;,
  4;58,58,58,58;,
  4;53,53,53,53;,
  4;54,54,54,54;,
  4;55,55,55,55;,
  4;56,56,56,56;,
  4;57,57,57,57;,
  4;58,58,58,58;,
  4;53,53,53,53;,
  4;54,54,54,54;,
  4;55,55,55,55;,
  4;56,56,56,56;,
  4;57,57,57,57;,
  4;58,58,58,58;,
  4;53,53,53,53;,
  4;54,54,54,54;,
  4;55,55,55,55;,
  4;56,56,56,56;,
  4;57,57,57,57;,
  4;58,58,58,58;,
  3;59,67,60;,
  3;60,67,61;,
  3;61,67,62;,
  3;62,96,63;,
  3;63,96,64;,
  3;64,96,65;,
  3;65,66,66;,
  3;66,66,59;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;69,77,70;,
  3;70,77,71;,
  3;71,77,72;,
  3;72,97,73;,
  3;73,97,74;,
  3;74,97,75;,
  3;75,76,76;,
  3;76,76,69;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;78,86,79;,
  3;79,86,80;,
  3;80,86,81;,
  3;81,98,82;,
  3;82,98,83;,
  3;83,98,84;,
  3;84,85,85;,
  3;85,85,78;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;87,95,88;,
  3;88,95,89;,
  3;89,95,90;,
  3;90,99,91;,
  3;91,99,92;,
  3;92,99,93;,
  3;93,94,94;,
  3;94,94,87;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;,
  3;68,68,68;;
 }
 MeshTextureCoords {
  388;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
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
  0.000000;0.000000;,
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
  0.000000;0.000000;,
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
  0.000000;0.000000;,
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
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
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
  0.000000;0.000000;,
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
  0.000000;0.000000;,
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
  0.000000;0.000000;,
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
