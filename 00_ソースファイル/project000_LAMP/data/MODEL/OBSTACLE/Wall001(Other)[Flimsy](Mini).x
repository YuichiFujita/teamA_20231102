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
 -25.17014;20.57756;-7.28943;,
 25.14019;20.57756;-7.28943;,
 25.14019;-0.02365;-7.28943;,
 -25.17014;-0.02365;-7.28943;,
 25.14019;20.57756;-7.28943;,
 25.14019;20.57756;7.38398;,
 25.14019;-0.02365;7.38398;,
 25.14019;-0.02365;-7.28943;,
 25.14019;20.57756;7.38398;,
 -25.17014;20.57756;7.38398;,
 -25.17014;-0.02365;7.38398;,
 25.14019;-0.02365;7.38398;,
 -25.17014;20.57756;7.38398;,
 -25.17014;20.57756;-7.28943;,
 -25.17014;-0.02365;-7.28943;,
 -25.17014;-0.02365;7.38398;,
 25.14019;20.57756;-7.28943;,
 -25.17014;20.57756;-7.28943;,
 -25.17014;-0.02365;-7.28943;,
 25.14019;-0.02365;-7.28943;,
 -25.17045;26.55588;2.86365;,
 -18.88395;26.55588;2.86365;,
 -18.88395;20.51580;2.86365;,
 -25.17045;20.51580;2.86365;,
 -18.88395;26.55588;2.86365;,
 -18.88395;26.55588;7.47687;,
 -18.88395;20.51580;7.47687;,
 -18.88395;20.51580;2.86365;,
 -18.88395;26.55588;7.47687;,
 -25.17045;26.55588;7.47687;,
 -25.17045;20.51580;7.47687;,
 -18.88395;20.51580;7.47687;,
 -25.17045;26.55588;7.47687;,
 -25.17045;26.55588;2.86365;,
 -25.17045;20.51580;2.86365;,
 -25.17045;20.51580;7.47687;,
 -18.88395;26.55588;2.86365;,
 -25.17045;26.55588;2.86365;,
 -25.17045;20.51580;2.86365;,
 -18.88395;20.51580;2.86365;,
 -10.55438;26.55588;2.86365;,
 -4.26788;26.55588;2.86365;,
 -4.26788;20.51580;2.86365;,
 -10.55438;20.51580;2.86365;,
 -4.26788;26.55588;2.86365;,
 -4.26788;26.55588;7.47687;,
 -4.26788;20.51580;7.47687;,
 -4.26788;20.51580;2.86365;,
 -4.26788;26.55588;7.47687;,
 -10.55438;26.55588;7.47687;,
 -10.55438;20.51580;7.47687;,
 -4.26788;20.51580;7.47687;,
 -10.55438;26.55588;7.47687;,
 -10.55438;26.55588;2.86365;,
 -10.55438;20.51580;2.86365;,
 -10.55438;20.51580;7.47687;,
 -4.26788;26.55588;2.86365;,
 -10.55438;26.55588;2.86365;,
 -10.55438;20.51580;2.86365;,
 -4.26788;20.51580;2.86365;,
 4.86119;26.55588;2.86365;,
 11.14770;26.55588;2.86365;,
 11.14770;20.51580;2.86365;,
 4.86119;20.51580;2.86365;,
 11.14770;26.55588;2.86365;,
 11.14770;26.55588;7.47687;,
 11.14770;20.51580;7.47687;,
 11.14770;20.51580;2.86365;,
 11.14770;26.55588;7.47687;,
 4.86119;26.55588;7.47687;,
 4.86119;20.51580;7.47687;,
 11.14770;20.51580;7.47687;,
 4.86119;26.55588;7.47687;,
 4.86119;26.55588;2.86365;,
 4.86119;20.51580;2.86365;,
 4.86119;20.51580;7.47687;,
 11.14770;26.55588;2.86365;,
 4.86119;26.55588;2.86365;,
 4.86119;20.51580;2.86365;,
 11.14770;20.51580;2.86365;,
 18.77770;26.55588;2.86365;,
 25.06421;26.55588;2.86365;,
 25.06421;20.51580;2.86365;,
 18.77770;20.51580;2.86365;,
 25.06421;26.55588;2.86365;,
 25.06421;26.55588;7.47687;,
 25.06421;20.51580;7.47687;,
 25.06421;20.51580;2.86365;,
 25.06421;26.55588;7.47687;,
 18.77770;26.55588;7.47687;,
 18.77770;20.51580;7.47687;,
 25.06421;20.51580;7.47687;,
 18.77770;26.55588;7.47687;,
 18.77770;26.55588;2.86365;,
 18.77770;20.51580;2.86365;,
 18.77770;20.51580;7.47687;,
 25.06421;26.55588;2.86365;,
 18.77770;26.55588;2.86365;,
 18.77770;20.51580;2.86365;,
 25.06421;20.51580;2.86365;,
 -7.42414;26.43809;1.01859;,
 -7.42414;30.88948;5.17027;,
 -4.51760;26.43809;2.23457;,
 -7.42414;30.88948;5.17027;,
 -3.31367;26.43809;5.17027;,
 -7.42414;30.88948;5.17027;,
 -4.51760;26.43809;8.10595;,
 -7.42414;30.88948;5.17027;,
 -7.42416;26.43809;9.32195;,
 -7.42414;30.88948;5.17027;,
 -10.33068;26.43809;8.10595;,
 -7.42414;30.88948;5.17027;,
 -11.53463;26.43809;5.17027;,
 -7.42414;30.88948;5.17027;,
 -10.33068;26.43809;2.23457;,
 -7.42414;30.88948;5.17027;,
 -7.42414;26.43809;1.01859;,
 -7.42414;26.43809;5.17027;,
 -7.42414;26.43809;1.01859;,
 -4.51760;26.43809;2.23457;,
 -3.31367;26.43809;5.17027;,
 -4.51760;26.43809;8.10595;,
 -7.42416;26.43809;9.32195;,
 -10.33068;26.43809;8.10595;,
 -11.53463;26.43809;5.17027;,
 -10.33068;26.43809;2.23457;,
 8.07225;26.43809;1.01859;,
 8.07225;30.88948;5.17027;,
 10.97881;26.43809;2.23457;,
 8.07225;30.88948;5.17027;,
 12.18274;26.43809;5.17027;,
 8.07225;30.88948;5.17027;,
 10.97881;26.43809;8.10595;,
 8.07225;30.88948;5.17027;,
 8.07225;26.43809;9.32195;,
 8.07225;30.88948;5.17027;,
 5.16573;26.43809;8.10595;,
 8.07225;30.88948;5.17027;,
 3.96178;26.43809;5.17027;,
 8.07225;30.88948;5.17027;,
 5.16573;26.43809;2.23457;,
 8.07225;30.88948;5.17027;,
 8.07225;26.43809;1.01859;,
 8.07225;26.43809;5.17027;,
 8.07225;26.43809;1.01859;,
 10.97881;26.43809;2.23457;,
 12.18274;26.43809;5.17027;,
 10.97881;26.43809;8.10595;,
 8.07225;26.43809;9.32195;,
 5.16573;26.43809;8.10595;,
 3.96178;26.43809;5.17027;,
 5.16573;26.43809;2.23457;,
 -22.23178;26.43809;1.01859;,
 -22.23178;30.88948;5.17027;,
 -19.32526;26.43809;2.23457;,
 -22.23178;30.88948;5.17027;,
 -18.12131;26.43809;5.17027;,
 -22.23178;30.88948;5.17027;,
 -19.32526;26.43809;8.10595;,
 -22.23178;30.88948;5.17027;,
 -22.23178;26.43809;9.32195;,
 -22.23178;30.88948;5.17027;,
 -25.13832;26.43809;8.10595;,
 -22.23178;30.88948;5.17027;,
 -26.34227;26.43809;5.17027;,
 -22.23178;30.88948;5.17027;,
 -25.13832;26.43809;2.23457;,
 -22.23178;30.88948;5.17027;,
 -22.23178;26.43809;1.01859;,
 -22.23178;26.43809;5.17027;,
 -22.23178;26.43809;1.01859;,
 -19.32526;26.43809;2.23457;,
 -18.12131;26.43809;5.17027;,
 -19.32526;26.43809;8.10595;,
 -22.23178;26.43809;9.32195;,
 -25.13832;26.43809;8.10595;,
 -26.34227;26.43809;5.17027;,
 -25.13832;26.43809;2.23457;,
 22.17135;26.43809;1.01859;,
 22.17135;30.88948;5.17027;,
 25.07789;26.43809;2.23457;,
 22.17135;30.88948;5.17027;,
 26.28182;26.43809;5.17027;,
 22.17135;30.88948;5.17027;,
 25.07789;26.43809;8.10595;,
 22.17135;30.88948;5.17027;,
 22.17135;26.43809;9.32195;,
 22.17135;30.88948;5.17027;,
 19.26483;26.43809;8.10595;,
 22.17135;30.88948;5.17027;,
 18.06088;26.43809;5.17027;,
 22.17135;30.88948;5.17027;,
 19.26483;26.43809;2.23457;,
 22.17135;30.88948;5.17027;,
 22.17135;26.43809;1.01859;,
 22.17135;26.43809;5.17027;,
 22.17135;26.43809;1.01859;,
 25.07789;26.43809;2.23457;,
 26.28182;26.43809;5.17027;,
 25.07789;26.43809;8.10595;,
 22.17135;26.43809;9.32195;,
 19.26483;26.43809;8.10595;,
 18.06088;26.43809;5.17027;,
 19.26483;26.43809;2.23457;,
 -25.17045;26.55588;-7.26696;,
 -18.88395;26.55588;-7.26696;,
 -18.88395;20.51580;-7.26696;,
 -25.17045;20.51580;-7.26696;,
 -18.88395;26.55588;-7.26696;,
 -18.88395;26.55588;-2.65374;,
 -18.88395;20.51580;-2.65374;,
 -18.88395;20.51580;-7.26696;,
 -18.88395;26.55588;-2.65374;,
 -25.17045;26.55588;-2.65374;,
 -25.17045;20.51580;-2.65374;,
 -18.88395;20.51580;-2.65374;,
 -25.17045;26.55588;-2.65374;,
 -25.17045;26.55588;-7.26696;,
 -25.17045;20.51580;-7.26696;,
 -25.17045;20.51580;-2.65374;,
 -18.88395;26.55588;-7.26696;,
 -25.17045;26.55588;-7.26696;,
 -25.17045;20.51580;-7.26696;,
 -18.88395;20.51580;-7.26696;,
 -10.55438;26.55588;-7.26696;,
 -4.26788;26.55588;-7.26696;,
 -4.26788;20.51580;-7.26696;,
 -10.55438;20.51580;-7.26696;,
 -4.26788;26.55588;-7.26696;,
 -4.26788;26.55588;-2.65374;,
 -4.26788;20.51580;-2.65374;,
 -4.26788;20.51580;-7.26696;,
 -4.26788;26.55588;-2.65374;,
 -10.55438;26.55588;-2.65374;,
 -10.55438;20.51580;-2.65374;,
 -4.26788;20.51580;-2.65374;,
 -10.55438;26.55588;-2.65374;,
 -10.55438;26.55588;-7.26696;,
 -10.55438;20.51580;-7.26696;,
 -10.55438;20.51580;-2.65374;,
 -4.26788;26.55588;-7.26696;,
 -10.55438;26.55588;-7.26696;,
 -10.55438;20.51580;-7.26696;,
 -4.26788;20.51580;-7.26696;,
 4.86119;26.55588;-7.26696;,
 11.14770;26.55588;-7.26696;,
 11.14770;20.51580;-7.26696;,
 4.86119;20.51580;-7.26696;,
 11.14770;26.55588;-7.26696;,
 11.14770;26.55588;-2.65374;,
 11.14770;20.51580;-2.65374;,
 11.14770;20.51580;-7.26696;,
 11.14770;26.55588;-2.65374;,
 4.86119;26.55588;-2.65374;,
 4.86119;20.51580;-2.65374;,
 11.14770;20.51580;-2.65374;,
 4.86119;26.55588;-2.65374;,
 4.86119;26.55588;-7.26696;,
 4.86119;20.51580;-7.26696;,
 4.86119;20.51580;-2.65374;,
 11.14770;26.55588;-7.26696;,
 4.86119;26.55588;-7.26696;,
 4.86119;20.51580;-7.26696;,
 11.14770;20.51580;-7.26696;,
 18.77770;26.55588;-7.26696;,
 25.06421;26.55588;-7.26696;,
 25.06421;20.51580;-7.26696;,
 18.77770;20.51580;-7.26696;,
 25.06421;26.55588;-7.26696;,
 25.06421;26.55588;-2.65374;,
 25.06421;20.51580;-2.65374;,
 25.06421;20.51580;-7.26696;,
 25.06421;26.55588;-2.65374;,
 18.77770;26.55588;-2.65374;,
 18.77770;20.51580;-2.65374;,
 25.06421;20.51580;-2.65374;,
 18.77770;26.55588;-2.65374;,
 18.77770;26.55588;-7.26696;,
 18.77770;20.51580;-7.26696;,
 18.77770;20.51580;-2.65374;,
 25.06421;26.55588;-7.26696;,
 18.77770;26.55588;-7.26696;,
 18.77770;20.51580;-7.26696;,
 25.06421;20.51580;-7.26696;,
 -7.42414;26.43809;-9.13122;,
 -7.42414;30.88948;-4.97954;,
 -4.51760;26.43809;-7.91522;,
 -7.42414;30.88948;-4.97954;,
 -3.31367;26.43809;-4.97954;,
 -7.42414;30.88948;-4.97954;,
 -4.51760;26.43809;-2.04386;,
 -7.42414;30.88948;-4.97954;,
 -7.42416;26.43809;-0.82786;,
 -7.42414;30.88948;-4.97954;,
 -10.33068;26.43809;-2.04386;,
 -7.42414;30.88948;-4.97954;,
 -11.53463;26.43809;-4.97954;,
 -7.42414;30.88948;-4.97954;,
 -10.33068;26.43809;-7.91522;,
 -7.42414;30.88948;-4.97954;,
 -7.42414;26.43809;-9.13122;,
 -7.42414;26.43809;-4.97954;,
 -7.42414;26.43809;-9.13122;,
 -4.51760;26.43809;-7.91522;,
 -3.31367;26.43809;-4.97954;,
 -4.51760;26.43809;-2.04386;,
 -7.42416;26.43809;-0.82786;,
 -10.33068;26.43809;-2.04386;,
 -11.53463;26.43809;-4.97954;,
 -10.33068;26.43809;-7.91522;,
 8.07225;26.43809;-9.13122;,
 8.07225;30.88948;-4.97954;,
 10.97881;26.43809;-7.91522;,
 8.07225;30.88948;-4.97954;,
 12.18274;26.43809;-4.97954;,
 8.07225;30.88948;-4.97954;,
 10.97881;26.43809;-2.04386;,
 8.07225;30.88948;-4.97954;,
 8.07225;26.43809;-0.82786;,
 8.07225;30.88948;-4.97954;,
 5.16573;26.43809;-2.04386;,
 8.07225;30.88948;-4.97954;,
 3.96178;26.43809;-4.97954;,
 8.07225;30.88948;-4.97954;,
 5.16573;26.43809;-7.91522;,
 8.07225;30.88948;-4.97954;,
 8.07225;26.43809;-9.13122;,
 8.07225;26.43809;-4.97954;,
 8.07225;26.43809;-9.13122;,
 10.97881;26.43809;-7.91522;,
 12.18274;26.43809;-4.97954;,
 10.97881;26.43809;-2.04386;,
 8.07225;26.43809;-0.82786;,
 5.16573;26.43809;-2.04386;,
 3.96178;26.43809;-4.97954;,
 5.16573;26.43809;-7.91522;,
 -22.23178;26.43809;-9.13122;,
 -22.23178;30.88948;-4.97954;,
 -19.32526;26.43809;-7.91522;,
 -22.23178;30.88948;-4.97954;,
 -18.12131;26.43809;-4.97954;,
 -22.23178;30.88948;-4.97954;,
 -19.32526;26.43809;-2.04386;,
 -22.23178;30.88948;-4.97954;,
 -22.23178;26.43809;-0.82786;,
 -22.23178;30.88948;-4.97954;,
 -25.13832;26.43809;-2.04386;,
 -22.23178;30.88948;-4.97954;,
 -26.34227;26.43809;-4.97954;,
 -22.23178;30.88948;-4.97954;,
 -25.13832;26.43809;-7.91522;,
 -22.23178;30.88948;-4.97954;,
 -22.23178;26.43809;-9.13122;,
 -22.23178;26.43809;-4.97954;,
 -22.23178;26.43809;-9.13122;,
 -19.32526;26.43809;-7.91522;,
 -18.12131;26.43809;-4.97954;,
 -19.32526;26.43809;-2.04386;,
 -22.23178;26.43809;-0.82786;,
 -25.13832;26.43809;-2.04386;,
 -26.34227;26.43809;-4.97954;,
 -25.13832;26.43809;-7.91522;,
 22.17135;26.43809;-9.13122;,
 22.17135;30.88948;-4.97954;,
 25.07789;26.43809;-7.91522;,
 22.17135;30.88948;-4.97954;,
 26.28182;26.43809;-4.97954;,
 22.17135;30.88948;-4.97954;,
 25.07789;26.43809;-2.04386;,
 22.17135;30.88948;-4.97954;,
 22.17135;26.43809;-0.82786;,
 22.17135;30.88948;-4.97954;,
 19.26483;26.43809;-2.04386;,
 22.17135;30.88948;-4.97954;,
 18.06088;26.43809;-4.97954;,
 22.17135;30.88948;-4.97954;,
 19.26483;26.43809;-7.91522;,
 22.17135;30.88948;-4.97954;,
 22.17135;26.43809;-9.13122;,
 22.17135;26.43809;-4.97954;,
 22.17135;26.43809;-9.13122;,
 25.07789;26.43809;-7.91522;,
 26.28182;26.43809;-4.97954;,
 25.07789;26.43809;-2.04386;,
 22.17135;26.43809;-0.82786;,
 19.26483;26.43809;-2.04386;,
 18.06088;26.43809;-4.97954;,
 19.26483;26.43809;-7.91522;;
 
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
    "data\\MODEL\\TEXTURE\\Rock-Wall002(Flimsy).png";
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
  99;
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
  0.000000;0.682059;-0.731297;,
  0.520451;0.680232;-0.516154;,
  0.734681;0.678413;0.000001;,
  0.520452;0.680231;0.516155;,
  -0.000001;0.682059;0.731297;,
  -0.520452;0.680231;0.516155;,
  -0.734680;0.678414;0.000001;,
  -0.520449;0.680232;-0.516156;,
  0.634012;0.727989;-0.260886;,
  0.000000;-1.000000;-0.000000;,
  -0.000002;0.682059;-0.731297;,
  0.520450;0.680233;-0.516154;,
  0.734680;0.678414;0.000001;,
  0.520451;0.680232;0.516154;,
  -0.000002;0.682059;0.731297;,
  -0.520452;0.680230;0.516156;,
  -0.734681;0.678413;0.000001;,
  -0.520451;0.680231;-0.516156;,
  0.634010;0.727990;-0.260886;,
  0.000001;0.682059;-0.731297;,
  0.520451;0.680231;-0.516156;,
  0.734681;0.678413;0.000001;,
  0.520452;0.680230;0.516156;,
  0.000001;0.682059;0.731297;,
  -0.520451;0.680231;0.516156;,
  -0.734680;0.678414;0.000001;,
  -0.520449;0.680232;-0.516156;,
  0.634012;0.727988;-0.260886;,
  -0.000001;0.682059;-0.731297;,
  0.520451;0.680232;-0.516154;,
  0.734681;0.678413;0.000001;,
  0.520452;0.680231;0.516155;,
  -0.000001;0.682059;0.731297;,
  -0.520452;0.680230;0.516156;,
  -0.734681;0.678413;0.000001;,
  0.634012;0.727989;-0.260886;,
  -0.262904;0.730284;0.630529;,
  -0.262905;0.730284;0.630529;,
  -0.262903;0.730284;0.630529;,
  -0.262905;0.730284;0.630529;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.682059;-0.731297;,
  0.520452;0.680231;-0.516154;,
  0.734681;0.678413;-0.000000;,
  0.520452;0.680231;0.516155;,
  -0.000001;0.682059;0.731297;,
  -0.520452;0.680231;0.516155;,
  -0.734680;0.678414;0.000000;,
  -0.520451;0.680231;-0.516156;,
  0.634013;0.727988;-0.260886;,
  0.000000;-1.000000;-0.000000;,
  -0.000002;0.682059;-0.731297;,
  0.520451;0.680232;-0.516154;,
  0.734680;0.678414;-0.000000;,
  0.520451;0.680232;0.516154;,
  -0.000002;0.682059;0.731297;,
  -0.520452;0.680230;0.516156;,
  -0.734681;0.678413;-0.000000;,
  -0.520452;0.680230;-0.516156;,
  0.634011;0.727989;-0.260887;,
  0.000001;0.682059;-0.731297;,
  0.520452;0.680230;-0.516156;,
  0.734681;0.678413;-0.000000;,
  0.520452;0.680230;0.516156;,
  0.000001;0.682059;0.731297;,
  -0.520451;0.680231;0.516156;,
  -0.734680;0.678414;-0.000000;,
  -0.520451;0.680231;-0.516156;,
  0.634013;0.727988;-0.260886;,
  -0.000001;0.682059;-0.731297;,
  0.520452;0.680231;-0.516155;,
  0.734681;0.678413;0.000000;,
  0.520452;0.680231;0.516154;,
  -0.000001;0.682059;0.731297;,
  -0.520452;0.680230;0.516156;,
  -0.734681;0.678413;-0.000000;,
  -0.520452;0.680230;-0.516156;,
  0.634013;0.727988;-0.260886;,
  -0.262904;0.730284;0.630529;,
  -0.262905;0.730284;0.630529;,
  -0.262903;0.730284;0.630529;,
  -0.262905;0.730284;0.630529;;
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
  3;15,48,16;,
  3;16,48,17;,
  3;17,48,18;,
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
  3;25,49,26;,
  3;26,49,27;,
  3;27,49,28;,
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
  3;34,50,35;,
  3;35,50,36;,
  3;36,50,37;,
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
  3;40,47,41;,
  3;41,47,42;,
  3;42,47,43;,
  3;43,51,44;,
  3;44,51,45;,
  3;45,51,46;,
  3;46,29,29;,
  3;29,29,40;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  4;52,52,52,52;,
  4;53,53,53,53;,
  4;54,54,54,54;,
  4;55,55,55,55;,
  4;56,56,56,56;,
  4;57,57,57,57;,
  4;52,52,52,52;,
  4;53,53,53,53;,
  4;54,54,54,54;,
  4;55,55,55,55;,
  4;56,56,56,56;,
  4;57,57,57,57;,
  4;52,52,52,52;,
  4;53,53,53,53;,
  4;54,54,54,54;,
  4;55,55,55,55;,
  4;56,56,56,56;,
  4;57,57,57,57;,
  4;52,52,52,52;,
  4;53,53,53,53;,
  4;54,54,54,54;,
  4;55,55,55,55;,
  4;56,56,56,56;,
  4;57,57,57,57;,
  3;58,66,59;,
  3;59,66,60;,
  3;60,66,61;,
  3;61,95,62;,
  3;62,95,63;,
  3;63,95,64;,
  3;64,65,65;,
  3;65,65,58;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;68,76,69;,
  3;69,76,70;,
  3;70,76,71;,
  3;71,96,72;,
  3;72,96,73;,
  3;73,96,74;,
  3;74,75,75;,
  3;75,75,68;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;77,85,78;,
  3;78,85,79;,
  3;79,85,80;,
  3;80,97,81;,
  3;81,97,82;,
  3;82,97,83;,
  3;83,84,84;,
  3;84,84,77;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;86,94,87;,
  3;87,94,88;,
  3;88,94,89;,
  3;89,98,90;,
  3;90,98,91;,
  3;91,98,92;,
  3;92,93,93;,
  3;93,93,86;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;;
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
