/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c9b94.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_c9b94(void)

{
  ushort uVar1;
  undefined4 uVar2;
  int iVar3;
  short sVar4;
  ushort unaff_D6w;
  ushort unaff_D7w;
  short sVar5;
  int unaff_A5;
  int unaff_A6;
  bool bVar6;

  bVar6 = false;
  candidate_aaae0();
  if (bVar6) {
    if (unaff_D6w == 0) {
      sVar5 = 0;
    }
    else if (unaff_D7w < unaff_D6w) {
      sVar5 = (short)(((uint)unaff_D7w * 0x3000) / (uint)unaff_D6w);
    }
    else {
      sVar5 = 0x3000;
    }
    sVar4 = *(short *)(unaff_A6 + 0x7e);
    *(short *)(unaff_A6 + 0x7e) = sVar5;
    if (sVar5 != sVar4) {
      if ((short)(sVar5 - sVar4) < 0) {
        *(byte *)(unaff_A6 + 0x89) = *(byte *)(unaff_A6 + 0x89) | 8;
      }
      else {
        *(byte *)(unaff_A6 + 0x89) = *(byte *)(unaff_A6 + 0x89) & 0xf7;
      }
    }
    if ((*(byte *)(unaff_A6 + 0x89) & 8) == 0) {
      sVar5 = *(short *)(unaff_A6 + 0x7e) - *(short *)(unaff_A6 + 0x7c);
      if (sVar5 == 0) {
        if (0x17ff < *(ushort *)(unaff_A6 + 0x7e)) {
          return;
        }
      }
      else {
        if (sVar5 < 0x201) {
          if (sVar5 < -0x200) {
            sVar5 = -0x200;
          }
        }
        else {
          sVar5 = 0x200;
        }
        *(short *)(unaff_A6 + 0x7c) = sVar5 + *(short *)(unaff_A6 + 0x7c);
      }
    }
    else {
      uVar1 = *(short *)(unaff_A6 + 0x7e) - *(short *)(unaff_A6 + 0x7c);
      if (uVar1 != 0) {
        *(short *)(unaff_A6 + 0x7c) =
             ((short)uVar1 >> 2) + ((short)uVar1 >> 1) + uVar1 + (ushort)((uVar1 & 1) != 0) +
             (ushort)(((short)uVar1 >> 1 & 1U) != 0) + *(short *)(unaff_A6 + 0x7c);
      }
    }
  }
  else {
    if (*(short *)(unaff_A6 + 0x7c) == 0) {
      return;
    }
    *(undefined2 *)(unaff_A6 + 0x7c) = 0;
    *(undefined2 *)(unaff_A6 + 0x7e) = 0;
  }
  sVar5 = *(short *)(unaff_A6 + 0x76) + 1;
  uVar2 = CONCAT22(sVar5,0x6b00);
  if (((*(ushort *)(unaff_A6 + 0x7e) != 0) && (*(ushort *)(unaff_A6 + 0x7e) < 0x1800)) &&
     ((*(ushort *)(unaff_A5 + -0x7ff2) & 7) == 0)) {
    uVar2 = CONCAT22(sVar5,0x7b00);
  }
  sVar5 = *(short *)(unaff_A6 + 0x7c);
  if (sVar5 < 0) {
    sVar5 = 0;
  }
  sVar4 = 6;
  iVar3 = CONCAT31((int3)((uint)(CONCAT31((int3)((uint)uVar2 >> 8),0xa5) + 0x200000) >> 8),0xae);
  do {
    sVar5 = sVar5 + -0x800;
    if (sVar5 < 0) {
      iVar3 = CONCAT31((int3)((uint)iVar3 >> 8),(char)((ushort)sVar5 >> 8) + (char)iVar3) + 0x200000
      ;
      sVar4 = sVar4 + -1;
      if (sVar4 != 0) {
        iVar3 = CONCAT31((int3)((uint)iVar3 >> 8),0xa6);
        do {
          iVar3 = iVar3 + 0x200000;
          sVar4 = sVar4 + -1;
        } while (sVar4 != 0);
      }
      break;
    }
    iVar3 = iVar3 + 0x200000;
    sVar4 = sVar4 + -1;
  } while (sVar4 != 0);
  _DAT_003c0000 = CONCAT31((int3)((uint)iVar3 >> 8),0xaf);
  return;
}
