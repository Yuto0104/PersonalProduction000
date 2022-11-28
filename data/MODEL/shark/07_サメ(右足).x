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
 53;
 -0.75503;0.57736;-1.35621;,
 -2.51028;0.81030;-0.74666;,
 -2.77828;1.68103;-1.74687;,
 -1.11237;1.73834;-2.68983;,
 -2.82812;2.45107;-1.93288;,
 -1.17882;2.76505;-2.93783;,
 -2.63061;2.66931;-1.19573;,
 -0.91547;3.05604;-1.95496;,
 -2.30142;2.20792;0.03276;,
 -0.47657;2.44085;-0.31697;,
 -2.03343;1.33718;1.03297;,
 -0.11923;1.27987;1.01664;,
 -1.98358;0.56715;1.21899;,
 -0.05278;0.25316;1.26465;,
 -2.18110;0.34891;0.48183;,
 -0.31613;-0.03783;0.28179;,
 -2.51028;0.81030;-0.74666;,
 -0.75503;0.57736;-1.35621;,
 -5.30707;0.84183;0.50311;,
 -5.75140;1.40292;-0.05772;,
 -5.89466;1.90824;-0.13784;,
 -5.65295;2.06177;0.30971;,
 -5.16784;1.77358;1.02273;,
 -4.72350;1.21249;1.58355;,
 -4.58023;0.70718;1.66366;,
 -4.82195;0.55364;1.21613;,
 -5.30707;0.84183;0.50311;,
 -6.79267;0.69138;1.72788;,
 -7.15512;0.93215;1.55639;,
 -7.28483;1.16833;1.56143;,
 -7.10589;1.26157;1.74010;,
 -6.72305;1.15726;1.98769;,
 -6.36063;0.91649;2.15920;,
 -6.23090;0.68031;2.15414;,
 -6.40986;0.58706;1.97549;,
 -6.79267;0.69138;1.72788;,
 -7.91002;0.39126;3.12237;,
 -7.91002;0.39126;3.12237;,
 -7.91002;0.39126;3.12237;,
 -7.91002;0.39126;3.12237;,
 -7.91002;0.39126;3.12237;,
 -7.91002;0.39126;3.12237;,
 -7.91002;0.39126;3.12237;,
 -7.91002;0.39126;3.12237;,
 -0.61580;1.50911;-0.83659;,
 -0.75503;0.57736;-1.35621;,
 -1.11237;1.73834;-2.68983;,
 -1.17882;2.76505;-2.93783;,
 -0.91547;3.05604;-1.95496;,
 -0.47657;2.44085;-0.31697;,
 -0.11923;1.27987;1.01664;,
 -0.05278;0.25316;1.26465;,
 -0.31613;-0.03783;0.28179;;
 
 40;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;1,18,19,2;,
 4;2,19,20,4;,
 4;4,20,21,6;,
 4;6,21,22,8;,
 4;8,22,23,10;,
 4;10,23,24,12;,
 4;12,24,25,14;,
 4;14,25,26,16;,
 4;18,27,28,19;,
 4;19,28,29,20;,
 4;20,29,30,21;,
 4;21,30,31,22;,
 4;22,31,32,23;,
 4;23,32,33,24;,
 4;24,33,34,25;,
 4;25,34,35,26;,
 3;27,36,28;,
 3;28,37,29;,
 3;29,38,30;,
 3;30,39,31;,
 3;31,40,32;,
 3;32,41,33;,
 3;33,42,34;,
 3;34,43,35;,
 3;44,45,46;,
 3;44,46,47;,
 3;44,47,48;,
 3;44,48,49;,
 3;44,49,50;,
 3;44,50,51;,
 3;44,51,52;,
 3;44,52,45;;
 
 MeshMaterialList {
  5;
  40;
  4,
  4,
  1,
  1,
  1,
  1,
  4,
  4,
  4,
  4,
  1,
  1,
  1,
  1,
  4,
  4,
  4,
  4,
  1,
  1,
  1,
  1,
  4,
  4,
  4,
  4,
  1,
  1,
  1,
  1,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.194510;0.304314;0.461176;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\3D表\現技術(後期)\\作品\\Ver.1.1(試作3号)\\data\\TEXTURE\\口.png";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  52;
  -0.266810;-0.857046;-0.440778;,
  -0.406570;-0.521739;-0.749993;,
  -0.472558;-0.235717;-0.849192;,
  -0.182256;0.979382;0.087145;,
  -0.010673;0.857046;0.515129;,
  0.022899;0.521739;0.852798;,
  0.015351;0.235719;0.971700;,
  -0.201414;-0.979381;0.015658;,
  -0.238229;-0.863549;-0.444444;,
  -0.392997;-0.528818;-0.752266;,
  -0.465556;-0.243107;-0.850974;,
  -0.167269;0.981780;0.090157;,
  0.018866;0.856958;0.515041;,
  0.086873;0.519192;0.850231;,
  0.098924;0.230500;0.968031;,
  -0.138690;-0.990300;0.008422;,
  -0.219662;-0.880853;-0.419340;,
  -0.451056;-0.564971;-0.690910;,
  -0.566696;-0.289067;-0.771554;,
  -0.222950;0.961545;0.160391;,
  0.037550;0.840448;0.540590;,
  0.183812;0.497772;0.847606;,
  0.236031;0.205488;0.949770;,
  -0.039868;-0.999123;0.012823;,
  -0.221773;-0.898673;-0.378422;,
  -0.548189;-0.619305;-0.562095;,
  -0.715914;-0.366334;-0.594362;,
  -0.320751;0.902728;0.286706;,
  0.034392;0.815603;0.577589;,
  0.272479;0.492548;0.826530;,
  0.374248;0.210209;0.903189;,
  0.049634;-0.998754;-0.005132;,
  -0.425502;-0.789378;-0.442527;,
  0.965926;0.000001;-0.258817;,
  -0.297742;0.934144;-0.196786;,
  -0.289098;0.937759;-0.192433;,
  -0.159462;-0.934143;0.319294;,
  -0.077059;-0.947197;0.311256;,
  -0.386477;0.916973;-0.098975;,
  0.063347;-0.950147;0.305300;,
  -0.535525;0.840831;0.078846;,
  0.201699;-0.944520;0.259228;,
  -0.012153;0.767995;0.640341;,
  0.280984;-0.936997;0.207568;,
  0.965927;0.000001;-0.258817;,
  0.965927;0.000001;-0.258816;,
  0.965927;0.000004;-0.258815;,
  0.965926;0.000003;-0.258817;,
  0.965926;0.000001;-0.258819;,
  0.965926;-0.000001;-0.258819;,
  0.965926;-0.000000;-0.258818;,
  0.965927;0.000002;-0.258817;;
  40;
  4;0,8,9,1;,
  4;1,9,10,2;,
  4;34,35,11,3;,
  4;3,11,12,4;,
  4;4,12,13,5;,
  4;5,13,14,6;,
  4;36,37,15,7;,
  4;7,15,8,0;,
  4;8,16,17,9;,
  4;9,17,18,10;,
  4;35,38,19,11;,
  4;11,19,20,12;,
  4;12,20,21,13;,
  4;13,21,22,14;,
  4;37,39,23,15;,
  4;15,23,16,8;,
  4;16,24,25,17;,
  4;17,25,26,18;,
  4;38,40,27,19;,
  4;19,27,28,20;,
  4;20,28,29,21;,
  4;21,29,30,22;,
  4;39,41,31,23;,
  4;23,31,24,16;,
  3;24,32,25;,
  3;25,32,26;,
  3;40,42,27;,
  3;27,42,28;,
  3;28,42,29;,
  3;29,42,30;,
  3;41,43,31;,
  3;31,32,24;,
  3;33,44,45;,
  3;33,45,46;,
  3;33,46,47;,
  3;33,47,48;,
  3;33,48,49;,
  3;33,49,50;,
  3;33,50,51;,
  3;33,51,44;;
 }
 MeshTextureCoords {
  53;
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
