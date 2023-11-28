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
 65;
 0.70885;21.97711;0.79187;,
 4.34393;20.95454;-2.84321;,
 0.70885;20.95454;-4.34891;,
 5.84961;20.95454;0.79187;,
 4.34393;21.82914;4.42693;,
 0.70885;21.82914;5.93263;,
 -2.92625;21.82914;4.42693;,
 -4.43194;20.95454;0.79187;,
 -2.92625;20.95454;-2.84321;,
 7.42559;18.91712;-5.92487;,
 0.70885;18.91712;-8.70704;,
 10.20776;18.91712;0.79187;,
 7.42559;18.91712;7.50861;,
 0.70885;18.91712;10.29078;,
 -6.00790;18.91712;7.50861;,
 -8.79007;18.91712;0.79187;,
 -6.00790;18.91712;-5.92487;,
 0.70885;20.95454;-4.34891;,
 0.70885;18.91712;-8.70704;,
 9.48469;14.55898;-7.98398;,
 0.70885;14.55898;-11.61906;,
 11.31897;14.55898;0.79187;,
 9.48469;14.55898;9.56772;,
 0.70885;14.55898;13.20280;,
 -8.06700;14.55898;9.56772;,
 -11.70209;14.55898;0.79187;,
 -8.06700;14.55898;-7.98398;,
 0.70885;14.55898;-11.61906;,
 10.20776;9.41821;-8.70704;,
 0.70885;9.41821;-12.64162;,
 12.75415;9.41821;0.79187;,
 10.20776;9.41821;10.29078;,
 0.70885;9.41821;14.22536;,
 -8.79008;9.41821;10.29078;,
 -12.72463;7.58776;0.79187;,
 -8.79008;9.41821;-8.70704;,
 0.70885;9.41821;-12.64162;,
 9.48469;4.27743;-7.98398;,
 0.70885;4.27743;-11.61906;,
 12.02876;4.27743;0.79187;,
 9.48469;4.27743;9.56772;,
 1.27707;4.32482;13.18149;,
 -7.49879;4.32482;9.54639;,
 -12.65914;1.80396;0.79187;,
 -8.06700;4.27743;-7.98398;,
 0.70885;4.27743;-11.61906;,
 7.42559;-0.08070;-5.92487;,
 0.70885;-0.08070;-8.70704;,
 10.20776;-0.08070;0.79187;,
 7.42559;-0.08070;7.50861;,
 1.27707;-0.03331;10.26945;,
 -7.29429;-0.03331;7.48728;,
 -10.45206;-0.09785;0.79187;,
 -6.00790;-0.08070;-5.92487;,
 0.70885;-0.08070;-8.70704;,
 4.34393;0.45398;-2.84321;,
 0.70885;0.45398;-4.34891;,
 5.84961;0.45398;0.79187;,
 4.34393;0.45398;4.42693;,
 0.70885;0.45398;5.93263;,
 -2.92625;0.45398;4.42693;,
 -4.43194;0.45398;0.79187;,
 -2.92625;0.45398;-2.84321;,
 0.70885;0.45398;-4.34891;,
 0.70885;-0.46302;0.79187;;
 
 64;
 3;0,1,2;,
 3;0,3,1;,
 3;0,4,3;,
 3;0,5,4;,
 3;0,6,5;,
 3;0,7,6;,
 3;0,8,7;,
 3;0,2,8;,
 4;2,1,9,10;,
 4;1,3,11,9;,
 4;3,4,12,11;,
 4;4,5,13,12;,
 4;5,6,14,13;,
 4;6,7,15,14;,
 4;7,8,16,15;,
 4;8,17,18,16;,
 4;10,9,19,20;,
 4;9,11,21,19;,
 4;11,12,22,21;,
 4;12,13,23,22;,
 4;13,14,24,23;,
 4;14,15,25,24;,
 4;15,16,26,25;,
 4;16,18,27,26;,
 4;20,19,28,29;,
 4;19,21,30,28;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;25,26,35,34;,
 4;26,27,36,35;,
 4;29,28,37,38;,
 4;28,30,39,37;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;34,35,44,43;,
 4;35,36,45,44;,
 4;38,37,46,47;,
 4;37,39,48,46;,
 4;39,40,49,48;,
 4;40,41,50,49;,
 4;41,42,51,50;,
 4;42,43,52,51;,
 4;43,44,53,52;,
 4;44,45,54,53;,
 4;47,46,55,56;,
 4;46,48,57,55;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 4;52,53,62,61;,
 4;53,54,63,62;,
 3;56,55,64;,
 3;55,57,64;,
 3;57,58,64;,
 3;58,59,64;,
 3;59,60,64;,
 3;60,61,64;,
 3;61,62,64;,
 3;62,63,64;;
 
 MeshMaterialList {
  1;
  64;
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
    "data\\MODEL\\TEXTURE\\Rock-Design(Cracked)000.png";
   }
  }
 }
 MeshNormals {
  65;
  0.000000;0.994855;-0.101305;,
  0.000000;0.950452;-0.310871;,
  0.219819;0.950452;-0.219819;,
  0.341277;0.937278;-0.070990;,
  0.254653;0.956219;0.144211;,
  0.000000;0.953784;0.300494;,
  -0.254653;0.956219;0.144212;,
  -0.341277;0.937278;-0.070990;,
  -0.219819;0.950452;-0.219819;,
  0.000000;0.762374;-0.647137;,
  0.497699;0.737428;-0.456614;,
  0.743277;0.668874;-0.012113;,
  0.540276;0.694396;0.475305;,
  0.000000;0.709230;0.704977;,
  -0.500751;0.722224;0.477118;,
  -0.672196;0.740278;-0.011845;,
  -0.457595;0.762374;-0.457595;,
  0.000000;0.384551;-0.923104;,
  0.705862;0.357507;-0.611512;,
  0.953573;0.301161;0.000000;,
  0.705863;0.357507;0.611512;,
  0.000000;0.384551;0.923104;,
  -0.656666;0.379409;0.651797;,
  -0.928039;0.372484;0.000000;,
  -0.656666;0.379409;-0.651797;,
  0.000000;0.000000;-1.000000;,
  0.754271;0.018458;-0.656304;,
  0.999417;0.034139;0.000000;,
  0.755677;0.023250;0.654531;,
  0.003943;-0.007793;0.999962;,
  -0.692864;-0.009142;0.721011;,
  -0.999763;0.020294;0.007891;,
  -0.698995;0.016709;-0.714931;,
  0.000000;-0.384551;-0.923104;,
  0.693029;-0.357888;-0.625801;,
  0.950837;-0.309691;0.000000;,
  0.699175;-0.352507;0.622008;,
  0.014488;-0.377103;0.926058;,
  -0.608620;-0.367154;0.703406;,
  -0.915294;-0.402779;-0.002508;,
  -0.601406;-0.380000;-0.702788;,
  0.000000;-0.555571;-0.831469;,
  0.629657;-0.514455;-0.582124;,
  0.890197;-0.455577;0.000000;,
  0.636525;-0.512843;0.576046;,
  0.020669;-0.528623;0.848605;,
  -0.538694;-0.522762;0.660704;,
  -0.350647;-0.936493;-0.005238;,
  -0.493401;-0.581008;-0.647290;,
  0.000000;-0.999632;-0.027117;,
  0.019175;-0.999632;-0.019174;,
  0.027117;-0.999632;0.000000;,
  0.018951;-0.999574;0.022189;,
  -0.002348;-0.999475;0.032301;,
  -0.029603;-0.999241;0.025327;,
  -0.039250;-0.999217;0.004937;,
  -0.023022;-0.999569;-0.018229;,
  0.000000;-1.000000;0.000000;,
  -0.000000;-0.992558;0.121772;,
  -0.086106;-0.992558;0.086106;,
  -0.121772;-0.992558;0.000000;,
  -0.086515;-0.993024;-0.080113;,
  -0.004644;-0.993759;-0.111451;,
  0.065374;-0.995125;-0.073839;,
  0.078457;-0.993032;0.087930;;
  64;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,1,9,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,17,25,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,25,33,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,33,41,48;,
  4;58,59,50,49;,
  4;59,60,51,50;,
  4;60,61,52,51;,
  4;61,62,53,52;,
  4;62,63,54,53;,
  4;63,47,55,54;,
  4;47,64,56,55;,
  4;64,58,49,56;,
  3;49,50,57;,
  3;50,51,57;,
  3;51,52,57;,
  3;52,53,57;,
  3;53,54,57;,
  3;54,55,57;,
  3;55,56,57;,
  3;56,49,57;;
 }
 MeshTextureCoords {
  65;
  0.453600;0.006760;,
  0.138560;0.072050;,
  0.004660;0.068560;,
  0.263740;0.077180;,
  0.381470;0.077870;,
  0.496100;0.078330;,
  0.612570;0.075420;,
  0.735510;0.073340;,
  0.867380;0.069130;,
  0.132200;0.142170;,
  0.002410;0.138960;,
  0.257540;0.147010;,
  0.378630;0.150600;,
  0.497810;0.151060;,
  0.618130;0.148150;,
  0.742240;0.143380;,
  0.871040;0.139480;,
  1.004660;0.068560;,
  1.002410;0.138960;,
  0.130480;0.217010;,
  0.001820;0.213950;,
  0.256760;0.197800;,
  0.377800;0.225020;,
  0.498300;0.225460;,
  0.619710;0.222690;,
  0.744080;0.218160;,
  0.872000;0.214450;,
  1.001820;0.213950;,
  0.130060;0.295600;,
  0.001680;0.292840;,
  0.255960;0.283170;,
  0.377600;0.302780;,
  0.498430;0.303170;,
  0.620110;0.300710;,
  0.744540;0.326760;,
  0.872240;0.293290;,
  1.001680;0.292840;,
  0.130480;0.381940;,
  0.001820;0.379810;,
  0.256340;0.375180;,
  0.377800;0.387340;,
  0.491270;0.386570;,
  0.614510;0.381350;,
  0.744510;0.445990;,
  0.872000;0.380160;,
  1.001820;0.379810;,
  0.132200;0.486860;,
  0.002410;0.486510;,
  0.257540;0.487350;,
  0.378630;0.487700;,
  0.488720;0.486240;,
  0.632560;0.487110;,
  0.743410;0.489430;,
  0.871040;0.486570;,
  1.002410;0.486510;,
  0.138560;0.441420;,
  0.004660;0.438410;,
  0.263740;0.445370;,
  0.381470;0.447950;,
  0.496100;0.448260;,
  0.612570;0.446220;,
  0.735510;0.442470;,
  0.867380;0.438920;,
  1.004660;0.438410;,
  0.453600;0.499200;;
 }
}
