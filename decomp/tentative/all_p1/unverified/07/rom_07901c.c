/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07901c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7901c(void)

{
  short sVar1;
  ushort uVar2;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_1af9a();
  sVar1 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0xf);
  if (sVar1 < 0) {
    if (-sVar1 < 0x30) {
LAB_0007830e:
      candidate_78f6e();
      *unaff_A6 = 0x78148;
      uVar2 = *(ushort *)(*(int *)(unaff_A6[3] + 0xc) + 0x4e);
      uVar3 = uVar2 < 500;
      if ((short)uVar2 < 0x1f5) {
        uVar2 = candidate_29c6();
        uVar3 = (uVar2 & 0xf) < 9;
        if (8 < (uVar2 & 0xf)) {
          *(byte *)(unaff_A6[3] + 0x9c) =
               *(byte *)(unaff_A6[3] + 0x9c) & ~('\x01' << (*(ushort *)((int)unaff_A6 + 0x8a) & 7));
          uVar2 = candidate_29c6();
          *(ushort *)((int)unaff_A6 + 0x8a) = (uVar2 & 1) * 2;
          candidate_1ad52();
          *(undefined2 *)((int)unaff_A6 + 0x6e) = 0xffff;
          *(undefined2 *)(unaff_A6 + 0x1c) = 0xffa0;
          if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
            *(short *)(unaff_A6 + 0x1c) = -*(short *)(unaff_A6 + 0x1c);
          }
          *(undefined2 *)((int)unaff_A6 + 0x72) = 0xffff;
          *(undefined2 *)(unaff_A6 + 0x1d) = 1;
          *(undefined2 *)((int)unaff_A6 + 0x76) = 0;
          *unaff_A6 = candidate_99374;
          return;
        }
      }
      candidate_78fd4();
      candidate_78fa2();
      candidate_78d7a();
      if ((bool)uVar3) {
        *unaff_A6 = 0x77e5c;
      }
      return;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      *unaff_A6 = candidate_782e8;
    }
    else {
      *unaff_A6 = candidate_782a4;
    }
  }
  else {
    if (sVar1 < 0x30) goto LAB_0007830e;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      *unaff_A6 = candidate_782a4;
    }
    else {
      *unaff_A6 = candidate_782e8;
    }
  }
  return;
}
