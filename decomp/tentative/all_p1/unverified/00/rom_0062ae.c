/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0062ae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_62ae(void)

{
  ushort uVar1;
  ushort uVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  ushort uVar6;
  ushort in_D0w;
  short sVar7;
  int iVar8;
  int in_A0;
  int in_A1;
  int unaff_A3;
  int unaff_A4;
  int unaff_A5;

  iVar8 = in_A0;
  if ((in_D0w != 0) && (iVar8 = in_A1, in_D0w < 0x1f)) {
    iVar8 = (short)(in_D0w << 6) + 0x51d8;
    uVar1 = *(ushort *)(in_A0 + 2);
    uVar2 = *(ushort *)(in_A1 + 2);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 2) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 4);
    uVar2 = *(ushort *)(in_A1 + 4);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 4) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 6);
    uVar2 = *(ushort *)(in_A1 + 6);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 6) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 8);
    uVar2 = *(ushort *)(in_A1 + 8);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 8) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 10);
    uVar2 = *(ushort *)(in_A1 + 10);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 10) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0xc);
    uVar2 = *(ushort *)(in_A1 + 0xc);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 0xc) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0xe);
    uVar2 = *(ushort *)(in_A1 + 0xe);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 0xe) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x10);
    uVar2 = *(ushort *)(in_A1 + 0x10);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 0x10) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x12);
    uVar2 = *(ushort *)(in_A1 + 0x12);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 0x12) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x14);
    uVar2 = *(ushort *)(in_A1 + 0x14);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 0x14) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x16);
    uVar2 = *(ushort *)(in_A1 + 0x16);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 0x16) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x18);
    uVar2 = *(ushort *)(in_A1 + 0x18);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 0x18) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x1a);
    uVar2 = *(ushort *)(in_A1 + 0x1a);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 0x1a) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x1c);
    uVar2 = *(ushort *)(in_A1 + 0x1c);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 0x1c) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x1e);
    uVar2 = *(ushort *)(in_A1 + 0x1e);
    uVar6 = uVar1 >> 5 & 0x1f;
    bVar3 = *(char *)(unaff_A5 + 0x191a) +
            (char)((uVar1 >> 10) + (ushort)*(byte *)(iVar8 + (short)((uVar2 >> 10) - (uVar1 >> 10)))
                  );
    bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
    bVar4 = *(char *)(unaff_A5 + 0x191c) +
            (char)uVar6 + *(char *)(iVar8 + (short)((uVar2 >> 5 & 0x1f) - uVar6));
    bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
    bVar5 = *(char *)(unaff_A5 + 0x191e) +
            (char)(uVar1 & 0x1f) + *(char *)(iVar8 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f)));
    bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
    sVar7 = ((byte)(-(0x1e < bVar3) | bVar3) & 0xff1f) << 5;
    sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5
    ;
    *(undefined2 *)(unaff_A4 + 0x1e) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7
                          ) * 2));
    return;
  }
  uVar1 = *(ushort *)(iVar8 + 2);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 2) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  uVar1 = *(ushort *)(iVar8 + 4);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 4) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  uVar1 = *(ushort *)(iVar8 + 6);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 6) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  uVar1 = *(ushort *)(iVar8 + 8);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 8) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  uVar1 = *(ushort *)(iVar8 + 10);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 10) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  uVar1 = *(ushort *)(iVar8 + 0xc);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 0xc) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  uVar1 = *(ushort *)(iVar8 + 0xe);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 0xe) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  uVar1 = *(ushort *)(iVar8 + 0x10);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 0x10) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  uVar1 = *(ushort *)(iVar8 + 0x12);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 0x12) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  uVar1 = *(ushort *)(iVar8 + 0x14);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 0x14) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  uVar1 = *(ushort *)(iVar8 + 0x16);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 0x16) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  uVar1 = *(ushort *)(iVar8 + 0x18);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 0x18) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  uVar1 = *(ushort *)(iVar8 + 0x1a);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 0x1a) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  uVar1 = *(ushort *)(iVar8 + 0x1c);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 0x1c) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  uVar1 = *(ushort *)(iVar8 + 0x1e);
  bVar3 = *(char *)(unaff_A5 + 0x191a) + ((byte)(uVar1 >> 10) & 0x1f);
  bVar3 = -(-1 < (short)((ushort)bVar3 << 8)) & bVar3;
  bVar4 = *(char *)(unaff_A5 + 0x191c) + ((byte)(uVar1 >> 5) & 0x1f);
  bVar4 = -(-1 < (short)((ushort)bVar4 << 8)) & bVar4;
  bVar5 = *(char *)(unaff_A5 + 0x191e) + ((byte)uVar1 & 0x1f);
  bVar5 = -(-1 < (short)((ushort)bVar5 << 8)) & bVar5;
  sVar7 = (ushort)((-(0x1e < bVar3) | bVar3) & 0x1f) << 5;
  sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar4) | bVar4) & 0x1f | (byte)sVar7) << 5;
  *(undefined2 *)(unaff_A4 + 0x1e) =
       *(undefined2 *)
        (unaff_A3 +
        (short)(CONCAT11((char)((ushort)sVar7 >> 8),(-(0x1e < bVar5) | bVar5) & 0x1f | (byte)sVar7)
               * 2));
  return;
}
