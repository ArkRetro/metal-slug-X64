/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x005ea0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5ea0(void)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  ushort in_D0w;
  short sVar4;
  int iVar5;
  int in_A0;
  int in_A1;
  int unaff_A3;
  int unaff_A4;

  iVar5 = in_A0;
  if ((in_D0w != 0) && (iVar5 = in_A1, in_D0w < 0x1f)) {
    iVar5 = (short)(in_D0w << 6) + 0x51d8;
    uVar1 = *(ushort *)(in_A0 + 2);
    uVar2 = *(ushort *)(in_A1 + 2);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 2) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 4);
    uVar2 = *(ushort *)(in_A1 + 4);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 4) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 6);
    uVar2 = *(ushort *)(in_A1 + 6);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 6) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 8);
    uVar2 = *(ushort *)(in_A1 + 8);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 8) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 10);
    uVar2 = *(ushort *)(in_A1 + 10);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 10) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0xc);
    uVar2 = *(ushort *)(in_A1 + 0xc);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 0xc) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0xe);
    uVar2 = *(ushort *)(in_A1 + 0xe);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 0xe) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x10);
    uVar2 = *(ushort *)(in_A1 + 0x10);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 0x10) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x12);
    uVar2 = *(ushort *)(in_A1 + 0x12);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 0x12) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x14);
    uVar2 = *(ushort *)(in_A1 + 0x14);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 0x14) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x16);
    uVar2 = *(ushort *)(in_A1 + 0x16);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 0x16) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x18);
    uVar2 = *(ushort *)(in_A1 + 0x18);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 0x18) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x1a);
    uVar2 = *(ushort *)(in_A1 + 0x1a);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 0x1a) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x1c);
    uVar2 = *(ushort *)(in_A1 + 0x1c);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 0x1c) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    uVar1 = *(ushort *)(in_A0 + 0x1e);
    uVar2 = *(ushort *)(in_A1 + 0x1e);
    uVar3 = uVar1 >> 5 & 0x1f;
    sVar4 = (uVar2 >> 10) - (uVar1 >> 10);
    sVar4 = ((uVar1 >> 10) + CONCAT11((char)((ushort)sVar4 >> 8),*(undefined1 *)(iVar5 + sVar4))) *
            0x20;
    sVar4 = CONCAT11((char)((ushort)sVar4 >> 8),
                     (char)uVar3 + *(char *)(iVar5 + (short)((uVar2 >> 5 & 0x1f) - uVar3)) |
                     (byte)sVar4) << 5;
    *(undefined2 *)(unaff_A4 + 0x1e) =
         *(undefined2 *)
          (unaff_A3 +
          (short)(CONCAT11((char)((ushort)sVar4 >> 8),
                           (char)(uVar1 & 0x1f) +
                           *(char *)(iVar5 + (short)((uVar2 & 0x1f) - (uVar1 & 0x1f))) | (byte)sVar4
                          ) * 2));
    return;
  }
  *(undefined2 *)(unaff_A4 + 2) = *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 2) * 2));
  *(undefined2 *)(unaff_A4 + 4) = *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 4) * 2));
  *(undefined2 *)(unaff_A4 + 6) = *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 6) * 2));
  *(undefined2 *)(unaff_A4 + 8) = *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 8) * 2));
  *(undefined2 *)(unaff_A4 + 10) = *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 10) * 2));
  *(undefined2 *)(unaff_A4 + 0xc) = *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 0xc) * 2))
  ;
  *(undefined2 *)(unaff_A4 + 0xe) = *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 0xe) * 2))
  ;
  *(undefined2 *)(unaff_A4 + 0x10) =
       *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 0x10) * 2));
  *(undefined2 *)(unaff_A4 + 0x12) =
       *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 0x12) * 2));
  *(undefined2 *)(unaff_A4 + 0x14) =
       *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 0x14) * 2));
  *(undefined2 *)(unaff_A4 + 0x16) =
       *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 0x16) * 2));
  *(undefined2 *)(unaff_A4 + 0x18) =
       *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 0x18) * 2));
  *(undefined2 *)(unaff_A4 + 0x1a) =
       *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 0x1a) * 2));
  *(undefined2 *)(unaff_A4 + 0x1c) =
       *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 0x1c) * 2));
  *(undefined2 *)(unaff_A4 + 0x1e) =
       *(undefined2 *)(unaff_A3 + (short)(*(short *)(iVar5 + 0x1e) * 2));
  return;
}
