/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0782e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_782e8(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4256();
  *unaff_A6 = 0x782fe;
  uVar2 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar2) {
    candidate_78f6e();
    *unaff_A6 = 0x78148;
    uVar1 = *(ushort *)(*(int *)(unaff_A6[3] + 0xc) + 0x4e);
    uVar2 = uVar1 < 500;
    if ((short)uVar1 < 0x1f5) {
      uVar1 = candidate_29c6();
      uVar2 = (uVar1 & 0xf) < 9;
      if (8 < (uVar1 & 0xf)) {
        *(byte *)(unaff_A6[3] + 0x9c) =
             *(byte *)(unaff_A6[3] + 0x9c) & ~('\x01' << (*(ushort *)((int)unaff_A6 + 0x8a) & 7));
        uVar1 = candidate_29c6();
        *(ushort *)((int)unaff_A6 + 0x8a) = (uVar1 & 1) * 2;
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
  }
  candidate_78fd4();
  candidate_78fa2();
  candidate_78d7a();
  if ((bool)uVar2) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
