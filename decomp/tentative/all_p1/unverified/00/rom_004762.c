/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x004762.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4762(void)

{
  int iVar1;
  short sVar2;
  short *psVar3;
  ushort uVar5;
  uint uVar4;
  short sVar7;
  short sVar8;
  uint uVar6;
  ushort uVar9;
  ushort uVar10;
  short unaff_D2w;
  ushort uVar11;
  byte bVar12;
  uint unaff_D3;
  undefined4 unaff_D4;
  undefined4 unaff_D5;
  undefined4 unaff_D6;
  undefined4 unaff_D7;
  int in_A0;
  int extraout_A0;
  short *psVar13;
  int unaff_A4;
  int unaff_A5;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A4 + (short)(unaff_D2w << 2));
  if (iVar1 != unaff_A6) {
    sVar7 = 0x58;
    if (-1 < *(char *)(unaff_A5 + -0x1c1e)) {
      sVar7 = 0x59;
    }
    if (((char)unaff_D2w == *(char *)(iVar1 + sVar7)) && (*(int *)(iVar1 + 0x50) != 0)) {
      psVar3 = (short *)(*(int *)(iVar1 + 0x50) + 2);
      do {
        do {
          psVar13 = psVar3;
          sVar7 = *psVar13;
          if (sVar7 == -0x8000) {
            return;
          }
          sVar2 = psVar13[3];
          sVar8 = (short)*(undefined4 *)(psVar13 + 1);
          if ((*(byte *)(iVar1 + 0x15) & 1) != 0) {
            sVar7 = -(sVar8 + sVar7);
          }
          uVar10 = (*(short *)(iVar1 + 0x3c) + sVar8 + sVar7) - (short)unaff_D4;
          uVar11 = (short)unaff_D5 + sVar8;
          uVar5 = (ushort)((uint)*(undefined4 *)(psVar13 + 1) >> 0x10);
          psVar3 = psVar13 + 4;
        } while (uVar11 <= uVar10);
        uVar6 = CONCAT22(uVar5,uVar11) << 0x10 | (uint)uVar5;
        if ((*(byte *)(iVar1 + 0x15) & 2) != 0) {
          uVar6 = CONCAT22(uVar11,-(sVar2 + uVar5));
        }
        uVar5 = (*(short *)(iVar1 + 0x40) + sVar2 + (short)uVar6) - (short)((uint)unaff_D4 >> 0x10);
        uVar11 = (short)((uint)unaff_D5 >> 0x10) + sVar2;
      } while (uVar11 <= uVar5);
      uVar9 = (ushort)psVar13[3] >> 1;
      uVar11 = uVar9 + (((short)uVar11 >> 1) - uVar5);
      if ((int)((uint)uVar11 << 0x10) < 0) {
        uVar11 = 0;
      }
      uVar5 = uVar11 - uVar9;
      if ((short)uVar9 < (short)(uVar11 - uVar9)) {
        uVar5 = uVar9;
      }
      sVar7 = psVar13[1];
      if ((*(byte *)(iVar1 + 0x15) & 2) != 0) {
        sVar7 = -sVar7;
      }
      *(ushort *)(unaff_A5 + -0x18ae) = sVar7 + uVar5;
      uVar5 = (ushort)psVar13[2] >> 1;
      uVar11 = uVar5 + ((short)((int)uVar6 >> 0x11) - uVar10);
      if ((int)((uint)uVar11 << 0x10) < 0) {
        uVar11 = 0;
      }
      uVar10 = uVar11 - uVar5;
      if ((short)uVar5 < (short)(uVar11 - uVar5)) {
        uVar10 = uVar5;
      }
      sVar7 = *psVar13;
      if ((*(byte *)(iVar1 + 0x15) & 1) != 0) {
        sVar7 = -sVar7;
      }
      *(ushort *)(unaff_A5 + -0x18b0) = sVar7 + uVar10;
      uVar11 = (ushort)**(byte **)(iVar1 + 0x50);
      if (*(ushort *)(unaff_A5 + -0x18ac) <= uVar11) {
        *(ushort *)(unaff_A5 + -0x18ac) = uVar11;
      }
      uVar6 = CONCAT22(unaff_D2w,uVar11 << 4);
      sVar7 = *(short *)(in_A0 + 8);
      if ((*(char *)((int)(short)(uVar11 << 4) + (short)(uVar11 << 2) + 0x4582 + (int)sVar7) != '\0'
          ) && ((*(int *)(iVar1 + 0x54) == 0 ||
                (*(short *)(*(int *)(iVar1 + 0x54) + (int)(short)(sVar7 * 2)) != 0)))) {
        if (*(byte *)(iVar1 + 0x5a) < (byte)sVar7) {
          *(byte *)(iVar1 + 0x5a) = (byte)sVar7;
          *(byte *)(iVar1 + 0x17) = *(byte *)(iVar1 + 0x17) | 6;
          *(undefined1 *)(iVar1 + 0x5b) = 0;
          *(byte *)(iVar1 + 0x5b) =
               -((int)((uint)(ushort)(*(short *)(iVar1 + 0x3c) - *(short *)(unaff_A6 + 0x44)) <<
                      0x10) < 0) & 0x80U | *(byte *)(iVar1 + 0x5b);
          uVar11 = *(short *)(iVar1 + 0x40) - *(short *)(unaff_A6 + 0x48);
          bVar12 = -((int)((uint)uVar11 << 0x10) < 0);
          uVar6 = CONCAT31((int3)(CONCAT22(unaff_D2w,uVar11) >> 8),bVar12) & 0xffffff40;
          *(byte *)(iVar1 + 0x5b) = bVar12 & 0x40 | *(byte *)(iVar1 + 0x5b);
          uVar4 = candidate_2eb4(uVar6);
          *(byte *)(iVar1 + 0x5b) = (byte)(uVar4 >> 4) & 0xf | *(byte *)(iVar1 + 0x5b);
          *(char *)(iVar1 + 0x5c) = (char)(*(short *)(unaff_A5 + -0x18b0) >> 1);
          *(char *)(iVar1 + 0x5d) = (char)(*(short *)(unaff_A5 + -0x18ae) >> 1);
        }
        if (((*(byte *)(iVar1 + 0x5f) & 0x80) == 0) && ((*(byte *)(iVar1 + 0x5f) & 0x40) == 0)) {
          uVar11 = *(ushort *)(iVar1 + 0x4e);
          if (*(ushort *)(unaff_A5 + -0x1826) <= uVar11) {
            *(ushort *)(unaff_A5 + -0x1826) = uVar11;
            *(int *)(unaff_A5 + -0x182a) = iVar1;
          }
          if (uVar11 <= *(ushort *)(unaff_A5 + -0x1820)) {
            *(ushort *)(unaff_A5 + -0x1820) = uVar11;
            *(int *)(unaff_A5 + -0x1824) = iVar1;
          }
          uVar10 = (ushort)unaff_D3;
          if (*(int *)(iVar1 + 0x54) != 0) {
            uVar10 = (ushort)((uint)*(ushort *)
                                     (*(int *)(iVar1 + 0x54) +
                                     (int)(short)(*(short *)(in_A0 + 8) * 2)) * (unaff_D3 & 0xffff)
                             >> 8);
          }
          if (uVar10 != 0) {
            sVar7 = uVar11 - uVar10;
            if (uVar11 < uVar10 || sVar7 == 0) {
              *(byte *)(iVar1 + 0x5f) = *(byte *)(iVar1 + 0x5f) | 0x80;
              if ((char)(unaff_D3 >> 0x10) == '\x01') {
                candidate_505a();
                in_A0 = extraout_A0;
              }
              sVar7 = 0;
            }
            *(short *)(iVar1 + 0x4e) = sVar7;
            if ((**(char **)(iVar1 + 0x50) != '\0') && (-1 < *(char *)(iVar1 + 0x5e))) {
              candidate_578fe(uVar6,unaff_D3,unaff_D4,unaff_D5,unaff_D6,unaff_D7,in_A0,
                              *(char **)(iVar1 + 0x50));
            }
            *(byte *)(iVar1 + 0x5e) =
                 *(byte *)(iVar1 + 0x5e) & 0xf3 | (*(byte *)(unaff_A6 + 0x5e) & 3) << 2;
          }
        }
      }
    }
  }
  return;
}
