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
 312;
 -0.31581;14.86790;-31.02940;,
 20.57279;14.86790;-21.93460;,
 10.98909;14.23290;-11.86070;,
 -0.31581;14.23290;-16.78280;,
 29.22520;14.86790;0.02230;,
 15.67169;14.23290;0.02230;,
 20.57279;14.86790;21.97910;,
 10.98909;14.23290;11.90520;,
 -0.31581;14.86790;31.07390;,
 -0.31581;14.23290;16.82730;,
 -21.20441;14.86790;21.97910;,
 -11.62071;14.23290;11.90520;,
 -29.85681;14.86790;0.02230;,
 -16.30331;14.23290;0.02230;,
 -21.20441;14.86790;-21.93460;,
 -11.62071;14.23290;-11.86070;,
 -0.31581;14.86790;-31.02940;,
 -0.31581;14.23290;-16.78280;,
 -0.31581;14.41190;0.02230;,
 -0.31581;14.41190;0.02230;,
 -0.31581;14.41190;0.02230;,
 -0.31581;14.41190;0.02230;,
 -0.31581;14.41190;0.02230;,
 -0.31581;14.41190;0.02230;,
 -0.31581;14.41190;0.02230;,
 -0.31581;14.41190;0.02230;,
 -0.31581;17.52900;-32.96990;,
 11.38639;17.52900;-30.78240;,
 11.38639;-7.96540;-30.78240;,
 -0.31581;-7.96540;-32.96990;,
 21.50819;17.52900;-24.51530;,
 21.50819;-7.96540;-24.51530;,
 28.68250;17.52900;-15.01500;,
 28.68250;-7.96540;-15.01500;,
 31.94039;17.52900;-3.56450;,
 31.94039;-7.96540;-3.56450;,
 30.84190;17.52900;8.28960;,
 30.84190;-7.96540;8.28960;,
 25.53549;17.52900;18.94640;,
 25.53549;-7.96540;18.94640;,
 16.73759;17.52900;26.96670;,
 16.73759;-7.96540;26.96670;,
 5.63669;17.52900;31.26720;,
 5.63669;-7.96540;31.26720;,
 -7.69441;17.90570;30.24410;,
 -6.26831;-7.96540;31.26720;,
 -17.36921;17.52900;26.96670;,
 -17.36921;-7.96540;26.96670;,
 -26.16711;17.52900;18.94640;,
 -26.16711;-7.96540;18.94640;,
 -31.47351;17.52900;8.28960;,
 -31.47351;-7.96540;8.28960;,
 -32.57201;17.52900;-3.56450;,
 -32.57201;-7.96540;-3.56450;,
 -29.31411;17.52900;-15.01500;,
 -29.31411;-7.96540;-15.01500;,
 -22.13971;17.52900;-24.51530;,
 -22.13971;-7.96540;-24.51530;,
 -12.01801;17.52900;-30.78240;,
 -12.01801;-7.96540;-30.78240;,
 -0.31581;17.52900;-32.96990;,
 -0.31581;-7.96540;-32.96990;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;17.52900;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 -0.31581;-7.96540;-0.57560;,
 32.42300;17.27150;14.99270;,
 32.42300;17.27150;7.87070;,
 32.42300;-13.54320;7.87070;,
 32.42300;-13.54320;14.99270;,
 32.42300;17.27150;0.74870;,
 32.42300;-13.54320;0.74870;,
 32.42300;17.27150;-6.37330;,
 32.42300;-13.54320;-6.37330;,
 32.42300;17.27150;-13.49530;,
 32.42300;-13.54320;-13.49530;,
 32.42300;17.27150;-20.61730;,
 32.42300;-13.54320;-20.61730;,
 32.42300;17.27150;-20.61730;,
 37.82809;17.27150;-20.61730;,
 37.82809;-13.54320;-20.61730;,
 32.42300;-13.54320;-20.61730;,
 37.82809;17.27150;-20.61730;,
 37.82809;17.27150;-13.49530;,
 37.82809;-13.54320;-13.49530;,
 37.82809;-13.54320;-20.61730;,
 37.82809;17.27150;-6.37330;,
 37.82809;-13.54320;-6.37330;,
 37.82809;17.27150;0.74870;,
 37.82809;-13.54320;0.74870;,
 37.82809;17.27150;7.87070;,
 37.82809;-13.54320;7.87070;,
 37.82809;17.27150;14.99270;,
 37.82809;-13.54320;14.99270;,
 37.82809;17.27150;14.99270;,
 32.42300;17.27150;14.99270;,
 32.42300;-13.54320;14.99270;,
 37.82809;-13.54320;14.99270;,
 37.82809;17.27150;7.87070;,
 32.42300;17.27150;7.87070;,
 32.42300;17.27150;14.99270;,
 37.82809;17.27150;0.74870;,
 32.42300;17.27150;0.74870;,
 37.82809;17.27150;-6.37330;,
 32.42300;17.27150;-6.37330;,
 37.82809;17.27150;-13.49530;,
 32.42300;17.27150;-13.49530;,
 32.42300;17.27150;-20.61730;,
 32.42300;-13.54320;14.99270;,
 32.42300;-13.54320;7.87070;,
 37.82809;-13.54320;7.87070;,
 32.42300;-13.54320;0.74870;,
 37.82809;-13.54320;0.74870;,
 32.42300;-13.54320;-6.37330;,
 37.82809;-13.54320;-6.37330;,
 32.42300;-13.54320;-13.49530;,
 37.82809;-13.54320;-13.49530;,
 32.42300;-13.54320;-20.61730;,
 -38.10071;17.27150;14.99270;,
 -38.10071;17.27150;7.87070;,
 -38.10071;-13.54320;7.87070;,
 -38.10071;-13.54320;14.99270;,
 -38.10071;17.27150;0.74870;,
 -38.10071;-13.54320;0.74870;,
 -38.10071;17.27150;-6.37330;,
 -38.10071;-13.54320;-6.37330;,
 -38.10071;17.27150;-13.49530;,
 -38.10071;-13.54320;-13.49530;,
 -38.10071;17.27150;-20.61730;,
 -38.10071;-13.54320;-20.61730;,
 -38.10071;17.27150;-20.61730;,
 -32.69571;17.27150;-20.61730;,
 -32.69571;-13.54320;-20.61730;,
 -38.10071;-13.54320;-20.61730;,
 -32.69571;17.27150;-20.61730;,
 -32.69571;17.27150;-13.49530;,
 -32.69571;-13.54320;-13.49530;,
 -32.69571;-13.54320;-20.61730;,
 -32.69571;17.27150;-6.37330;,
 -32.69571;-13.54320;-6.37330;,
 -32.69571;17.27150;0.74870;,
 -32.69571;-13.54320;0.74870;,
 -32.69571;17.27150;7.87070;,
 -32.69571;-13.54320;7.87070;,
 -32.69571;17.27150;14.99270;,
 -32.69571;-13.54320;14.99270;,
 -32.69571;17.27150;14.99270;,
 -38.10071;17.27150;14.99270;,
 -38.10071;-13.54320;14.99270;,
 -32.69571;-13.54320;14.99270;,
 -32.69571;17.27150;7.87070;,
 -38.10071;17.27150;7.87070;,
 -38.10071;17.27150;14.99270;,
 -32.69571;17.27150;0.74870;,
 -38.10071;17.27150;0.74870;,
 -32.69571;17.27150;-6.37330;,
 -38.10071;17.27150;-6.37330;,
 -32.69571;17.27150;-13.49530;,
 -38.10071;17.27150;-13.49530;,
 -38.10071;17.27150;-20.61730;,
 -38.10071;-13.54320;14.99270;,
 -38.10071;-13.54320;7.87070;,
 -32.69571;-13.54320;7.87070;,
 -38.10071;-13.54320;0.74870;,
 -32.69571;-13.54320;0.74870;,
 -38.10071;-13.54320;-6.37330;,
 -32.69571;-13.54320;-6.37330;,
 -38.10071;-13.54320;-13.49530;,
 -32.69571;-13.54320;-13.49530;,
 -38.10071;-13.54320;-20.61730;,
 -19.74570;17.68690;-34.57920;,
 0.02569;17.68690;-34.57920;,
 0.02569;7.80130;-34.57920;,
 -19.74570;7.80130;-34.57920;,
 19.79649;17.68690;-34.57920;,
 19.79649;7.80130;-34.57920;,
 0.02569;-2.08420;-34.57920;,
 -19.74570;-2.08420;-34.57920;,
 19.79649;-2.08420;-34.57920;,
 0.02569;-11.96970;-34.57920;,
 -19.74570;-11.96970;-34.57920;,
 19.79649;-11.96970;-34.57920;,
 0.02569;-21.85530;-34.57920;,
 0.01739;-21.85530;-34.57920;,
 0.05419;-21.85530;-34.57920;,
 19.79649;17.68690;-34.57920;,
 19.79649;17.68690;-33.80010;,
 19.79649;7.80130;-33.80010;,
 19.79649;7.80130;-34.57920;,
 19.79649;-2.08420;-33.80010;,
 19.79649;-2.08420;-34.57920;,
 19.79649;-11.96970;-33.80010;,
 19.79649;-11.96970;-34.57920;,
 0.05419;-21.85530;-33.80010;,
 0.05419;-21.85530;-34.57920;,
 19.79649;17.68690;-33.80010;,
 0.02569;17.68690;-33.80010;,
 0.02569;7.80130;-33.80010;,
 19.79649;7.80130;-33.80010;,
 -19.74570;17.68690;-33.80010;,
 -19.74570;7.80130;-33.80010;,
 0.02569;-2.08420;-33.80010;,
 19.79649;-2.08420;-33.80010;,
 -19.74570;-2.08420;-33.80010;,
 0.02569;-11.96970;-33.80010;,
 19.79649;-11.96970;-33.80010;,
 -19.74570;-11.96970;-33.80010;,
 0.02569;-21.85530;-33.80010;,
 0.05419;-21.85530;-33.80010;,
 0.01739;-21.85530;-33.80010;,
 -19.74570;17.68690;-33.80010;,
 -19.74570;17.68690;-34.57920;,
 -19.74570;7.80130;-34.57920;,
 -19.74570;7.80130;-33.80010;,
 -19.74570;-2.08420;-34.57920;,
 -19.74570;-2.08420;-33.80010;,
 -19.74570;-11.96970;-34.57920;,
 -19.74570;-11.96970;-33.80010;,
 0.01739;-21.85530;-34.57920;,
 0.01739;-21.85530;-33.80010;,
 0.02569;17.68690;-34.57920;,
 -19.74570;17.68690;-34.57920;,
 19.79649;17.68690;-34.57920;,
 0.01739;-21.85530;-34.57920;,
 0.02569;-21.85530;-34.57920;,
 0.05419;-21.85530;-34.57920;,
 -26.86930;17.86890;31.67130;,
 -0.33331;17.86890;31.67130;,
 -0.33331;4.60100;31.67130;,
 -26.86930;4.60100;31.67130;,
 26.20190;17.86890;31.67130;,
 26.20190;4.60100;31.67130;,
 -0.33331;-8.66670;31.67130;,
 -26.86930;-8.66670;31.67130;,
 26.20190;-8.66670;31.67130;,
 -0.33331;-21.93450;31.67130;,
 -26.86930;-21.93450;31.67130;,
 26.20190;-21.93450;31.67130;,
 -0.33331;-35.20230;31.67130;,
 -0.34441;-35.20230;31.67130;,
 -0.29511;-35.20230;31.67130;,
 26.20190;17.86890;31.67130;,
 26.20190;17.86890;32.71700;,
 26.20190;4.60100;32.71700;,
 26.20190;4.60100;31.67130;,
 26.20190;-8.66670;32.71700;,
 26.20190;-8.66670;31.67130;,
 26.20190;-21.93450;32.71700;,
 26.20190;-21.93450;31.67130;,
 -0.29511;-35.20230;32.71700;,
 -0.29511;-35.20230;31.67130;,
 26.20190;17.86890;32.71700;,
 -0.33331;17.86890;32.71700;,
 -0.33331;4.60100;32.71700;,
 26.20190;4.60100;32.71700;,
 -26.86930;17.86890;32.71700;,
 -26.86930;4.60100;32.71700;,
 -0.33331;-8.66670;32.71700;,
 26.20190;-8.66670;32.71700;,
 -26.86930;-8.66670;32.71700;,
 -0.33331;-21.93450;32.71700;,
 26.20190;-21.93450;32.71700;,
 -26.86930;-21.93450;32.71700;,
 -0.33331;-35.20230;32.71700;,
 -0.29511;-35.20230;32.71700;,
 -0.34441;-35.20230;32.71700;,
 -26.86930;17.86890;32.71700;,
 -26.86930;17.86890;31.67130;,
 -26.86930;4.60100;31.67130;,
 -26.86930;4.60100;32.71700;,
 -26.86930;-8.66670;31.67130;,
 -26.86930;-8.66670;32.71700;,
 -26.86930;-21.93450;31.67130;,
 -26.86930;-21.93450;32.71700;,
 -0.34441;-35.20230;31.67130;,
 -0.34441;-35.20230;32.71700;,
 -0.33331;17.86890;31.67130;,
 -26.86930;17.86890;31.67130;,
 26.20190;17.86890;31.67130;,
 -0.34441;-35.20230;31.67130;,
 -0.33331;-35.20230;31.67130;,
 -0.29511;-35.20230;31.67130;;
 
 167;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 3;3,2,18;,
 3;2,5,19;,
 3;5,7,20;,
 3;7,9,21;,
 3;9,11,22;,
 3;11,13,23;,
 3;13,15,24;,
 3;15,17,25;,
 4;26,27,28,29;,
 4;27,30,31,28;,
 4;30,32,33,31;,
 4;32,34,35,33;,
 4;34,36,37,35;,
 4;36,38,39,37;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;42,44,45,43;,
 4;44,46,47,45;,
 4;46,48,49,47;,
 4;48,50,51,49;,
 4;50,52,53,51;,
 4;52,54,55,53;,
 4;54,56,57,55;,
 4;56,58,59,57;,
 4;58,60,61,59;,
 3;62,27,26;,
 3;63,30,27;,
 3;64,32,30;,
 3;65,34,32;,
 3;66,36,34;,
 3;67,38,36;,
 3;68,40,38;,
 3;69,42,40;,
 3;70,44,42;,
 3;71,46,44;,
 3;72,48,46;,
 3;73,50,48;,
 3;74,52,50;,
 3;75,54,52;,
 3;76,56,54;,
 3;77,58,56;,
 3;78,60,58;,
 3;79,29,28;,
 3;80,28,31;,
 3;81,31,33;,
 3;82,33,35;,
 3;83,35,37;,
 3;84,37,39;,
 3;85,39,41;,
 3;86,41,43;,
 3;87,43,45;,
 3;88,45,47;,
 3;89,47,49;,
 3;90,49,51;,
 3;91,51,53;,
 3;92,53,55;,
 3;93,55,57;,
 3;94,57,59;,
 3;95,59,61;,
 4;96,97,98,99;,
 4;97,100,101,98;,
 4;100,102,103,101;,
 4;102,104,105,103;,
 4;104,106,107,105;,
 4;108,109,110,111;,
 4;112,113,114,115;,
 4;113,116,117,114;,
 4;116,118,119,117;,
 4;118,120,121,119;,
 4;120,122,123,121;,
 4;124,125,126,127;,
 4;124,128,129,130;,
 4;128,131,132,129;,
 4;131,133,134,132;,
 4;133,135,136,134;,
 4;135,109,137,136;,
 4;138,139,140,127;,
 4;139,141,142,140;,
 4;141,143,144,142;,
 4;143,145,146,144;,
 4;145,147,110,146;,
 4;148,149,150,151;,
 4;149,152,153,150;,
 4;152,154,155,153;,
 4;154,156,157,155;,
 4;156,158,159,157;,
 4;160,161,162,163;,
 4;164,165,166,167;,
 4;165,168,169,166;,
 4;168,170,171,169;,
 4;170,172,173,171;,
 4;172,174,175,173;,
 4;176,177,178,179;,
 4;176,180,181,182;,
 4;180,183,184,181;,
 4;183,185,186,184;,
 4;185,187,188,186;,
 4;187,161,189,188;,
 4;190,191,192,179;,
 4;191,193,194,192;,
 4;193,195,196,194;,
 4;195,197,198,196;,
 4;197,199,162,198;,
 4;200,201,202,203;,
 4;201,204,205,202;,
 4;203,202,206,207;,
 4;202,205,208,206;,
 4;207,206,209,210;,
 4;206,208,211,209;,
 4;210,209,212,213;,
 4;209,211,214,212;,
 4;215,216,217,218;,
 4;218,217,219,220;,
 4;220,219,221,222;,
 4;222,221,223,224;,
 4;225,226,227,228;,
 4;226,229,230,227;,
 4;228,227,231,232;,
 4;227,230,233,231;,
 4;232,231,234,235;,
 4;231,233,236,234;,
 4;235,234,237,238;,
 4;234,236,239,237;,
 4;240,241,242,243;,
 4;243,242,244,245;,
 4;245,244,246,247;,
 4;247,246,248,249;,
 4;240,226,250,251;,
 4;226,216,252,250;,
 4;253,254,237,249;,
 4;254,255,223,237;,
 4;256,257,258,259;,
 4;257,260,261,258;,
 4;259,258,262,263;,
 4;258,261,264,262;,
 4;263,262,265,266;,
 4;262,264,267,265;,
 4;266,265,268,269;,
 4;265,267,270,268;,
 4;271,272,273,274;,
 4;274,273,275,276;,
 4;276,275,277,278;,
 4;278,277,279,280;,
 4;281,282,283,284;,
 4;282,285,286,283;,
 4;284,283,287,288;,
 4;283,286,289,287;,
 4;288,287,290,291;,
 4;287,289,292,290;,
 4;291,290,293,294;,
 4;290,292,295,293;,
 4;296,297,298,299;,
 4;299,298,300,301;,
 4;301,300,302,303;,
 4;303,302,304,305;,
 4;296,282,306,307;,
 4;282,272,308,306;,
 4;309,310,293,305;,
 4;310,311,279,293;;
 
 MeshMaterialList {
  4;
  167;
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
  51;
  0.000000;-0.999008;-0.044527;,
  0.033094;-0.998956;-0.031485;,
  0.046800;-0.998904;0.000000;,
  0.033094;-0.998956;0.031485;,
  0.000000;-0.999008;0.044527;,
  -0.033094;-0.998956;0.031485;,
  -0.046800;-0.998904;0.000000;,
  -0.033094;-0.998956;-0.031485;,
  -0.000000;-0.999856;-0.016942;,
  0.012592;-0.999849;-0.011980;,
  0.017807;-0.999841;0.000000;,
  0.012592;-0.999849;0.011980;,
  0.000000;-0.999856;0.016942;,
  -0.012592;-0.999849;0.011980;,
  -0.017807;-0.999841;0.000000;,
  -0.012592;-0.999849;-0.011980;,
  0.000000;-1.000000;-0.000000;,
  -0.000206;0.999999;-0.001487;,
  0.000000;0.000000;-1.000000;,
  0.361239;0.000000;-0.932473;,
  0.673694;0.000000;-0.739010;,
  0.895164;0.000000;-0.445737;,
  0.995735;0.000000;-0.092264;,
  0.961826;0.000000;0.273663;,
  0.798018;0.000000;0.602634;,
  0.526431;0.000000;0.850218;,
  0.165154;0.009006;0.986227;,
  -0.191747;0.013220;0.981355;,
  -0.517263;0.004324;0.855815;,
  -0.798018;0.000000;0.602634;,
  -0.961826;0.000000;0.273663;,
  -0.995735;0.000000;-0.092264;,
  -0.895163;0.000000;-0.445740;,
  -0.673693;0.000000;-0.739011;,
  -0.361241;0.000000;-0.932472;,
  0.000000;-1.000000;-0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.230038;-0.973182;-0.000000;,
  0.230036;-0.973182;-0.000000;,
  0.000000;1.000000;0.000000;,
  0.014330;0.999894;-0.002679;,
  -0.001754;0.999919;-0.012642;,
  -0.016084;0.999821;-0.009959;,
  0.447737;-0.894165;-0.000000;,
  -0.447360;-0.894354;0.000000;,
  -0.229833;-0.973230;0.000000;,
  0.447734;-0.894167;-0.000000;,
  -0.447358;-0.894355;0.000000;,
  -0.229831;-0.973230;0.000000;;
  167;
  4;0,1,9,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,0,8,15;,
  3;8,9,16;,
  3;9,10,16;,
  3;10,11,16;,
  3;11,12,16;,
  3;12,13,16;,
  3;13,14,16;,
  3;14,15,16;,
  3;15,8,16;,
  4;18,19,19,18;,
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
  4;34,18,18,34;,
  3;17,41,41;,
  3;17,41,41;,
  3;17,41,41;,
  3;17,41,41;,
  3;17,41,41;,
  3;17,41,41;,
  3;17,41,41;,
  3;17,42,41;,
  3;17,43,42;,
  3;17,44,43;,
  3;17,41,44;,
  3;17,41,41;,
  3;17,41,41;,
  3;17,41,41;,
  3;17,41,41;,
  3;17,41,41;,
  3;17,41,41;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  3;35,35,35;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;18,18,18,18;,
  4;37,37,37,37;,
  4;37,37,37,37;,
  4;37,37,37,37;,
  4;37,37,37,37;,
  4;37,37,37,37;,
  4;38,38,38,38;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;35,35,35,35;,
  4;35,35,35,35;,
  4;35,35,35,35;,
  4;35,35,35,35;,
  4;35,35,35,35;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;18,18,18,18;,
  4;37,37,37,37;,
  4;37,37,37,37;,
  4;37,37,37,37;,
  4;37,37,37,37;,
  4;37,37,37,37;,
  4;38,38,38,38;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;35,35,35,35;,
  4;35,35,35,35;,
  4;35,35,35,35;,
  4;35,35,35,35;,
  4;35,35,35,35;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;37,37,37,37;,
  4;37,37,37,37;,
  4;37,37,37,37;,
  4;45,45,39,39;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;46,46,47,47;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;47,35,35,47;,
  4;35,39,39,35;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;37,37,37,37;,
  4;37,37,37,37;,
  4;37,37,37,37;,
  4;48,48,40,40;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;49,49,50,50;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;50,35,35,50;,
  4;35,40,40,35;;
 }
 MeshTextureCoords {
  312;
  0.000000;0.750000;,
  0.125000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.750000;,
  0.250000;0.875000;,
  0.375000;0.750000;,
  0.375000;0.875000;,
  0.500000;0.750000;,
  0.500000;0.875000;,
  0.625000;0.750000;,
  0.625000;0.875000;,
  0.750000;0.750000;,
  0.750000;0.875000;,
  0.875000;0.750000;,
  0.875000;0.875000;,
  1.000000;0.750000;,
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
  0.970590;1.000000;,
  0.000000;0.000000;,
  0.200000;0.000000;,
  0.200000;1.000000;,
  0.000000;1.000000;,
  0.400000;0.000000;,
  0.400000;1.000000;,
  0.600000;0.000000;,
  0.600000;1.000000;,
  0.800000;0.000000;,
  0.800000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.200000;0.000000;,
  0.200000;1.000000;,
  0.000000;1.000000;,
  0.400000;0.000000;,
  0.400000;1.000000;,
  0.600000;0.000000;,
  0.600000;1.000000;,
  0.800000;0.000000;,
  0.800000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.200000;0.000000;,
  0.200000;1.000000;,
  0.000000;1.000000;,
  0.400000;0.000000;,
  0.400000;1.000000;,
  0.600000;0.000000;,
  0.600000;1.000000;,
  0.800000;0.000000;,
  0.800000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.200000;0.000000;,
  0.200000;1.000000;,
  0.400000;0.000000;,
  0.400000;1.000000;,
  0.600000;0.000000;,
  0.600000;1.000000;,
  0.800000;0.000000;,
  0.800000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.200000;0.000000;,
  0.200000;1.000000;,
  0.000000;1.000000;,
  0.400000;0.000000;,
  0.400000;1.000000;,
  0.600000;0.000000;,
  0.600000;1.000000;,
  0.800000;0.000000;,
  0.800000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.200000;0.000000;,
  0.200000;1.000000;,
  0.000000;1.000000;,
  0.400000;0.000000;,
  0.400000;1.000000;,
  0.600000;0.000000;,
  0.600000;1.000000;,
  0.800000;0.000000;,
  0.800000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.200000;0.000000;,
  0.200000;1.000000;,
  0.000000;1.000000;,
  0.400000;0.000000;,
  0.400000;1.000000;,
  0.600000;0.000000;,
  0.600000;1.000000;,
  0.800000;0.000000;,
  0.800000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.200000;0.000000;,
  0.200000;1.000000;,
  0.400000;0.000000;,
  0.400000;1.000000;,
  0.600000;0.000000;,
  0.600000;1.000000;,
  0.800000;0.000000;,
  0.800000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.000000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  0.500000;0.750000;,
  0.000000;0.750000;,
  1.000000;0.750000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.000000;0.250000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.750000;,
  0.000000;0.750000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.000000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  0.500000;0.750000;,
  0.000000;0.750000;,
  1.000000;0.750000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.000000;0.250000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.750000;,
  0.000000;0.750000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.000000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  0.500000;0.750000;,
  0.000000;0.750000;,
  1.000000;0.750000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.000000;0.250000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.750000;,
  0.000000;0.750000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.000000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  0.500000;0.750000;,
  0.000000;0.750000;,
  1.000000;0.750000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.000000;0.250000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.750000;,
  0.000000;0.750000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;;
 }
}
