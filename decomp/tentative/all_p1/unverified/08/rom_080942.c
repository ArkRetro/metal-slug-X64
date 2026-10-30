/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x080942.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_80942(void)

{
  byte bVar1;
  char cVar2;
  char extraout_D1b;
  int unaff_A6;
  undefined1 uVar3;

  *(short *)(unaff_A6 + 0x82) = *(short *)(unaff_A6 + 0x82) + -1;
  uVar3 = 0;
  if (*(short *)(unaff_A6 + 0x82) < 1) {
    candidate_80a08();
    candidate_809c0();
    if ((bool)uVar3) {
      *(undefined1 *)(unaff_A6 + 0x8c) = 0x40;
    }
    else {
      uVar3 = candidate_366ac();
      *(undefined1 *)(unaff_A6 + 0x8c) = uVar3;
    }
    bVar1 = candidate_29c6();
    *(char *)(unaff_A6 + 0x8c) = ((bVar1 & 0xf) - 7) + *(char *)(unaff_A6 + 0x8c);
  }
  *(short *)(unaff_A6 + 0x84) = *(short *)(unaff_A6 + 0x84) + 1;
  if ((*(ushort *)(unaff_A6 + 0x84) & 7) == 0) {
    uVar3 = 6;
    if ((short)((ushort)(byte)(*(char *)(unaff_A6 + 0x8c) - *(char *)(unaff_A6 + 0x8a)) << 8) < 0) {
      uVar3 = 0xfa;
    }
    *(undefined1 *)(unaff_A6 + 0x8b) = uVar3;
  }
  cVar2 = candidate_29f2();
  *(char *)(unaff_A6 + 0x8a) = cVar2 + -1 + extraout_D1b + *(char *)(unaff_A6 + 0x8a);
  candidate_809d6();
  return;
}
