/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x004e76.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4e76(void)

{
  byte bVar1;
  char cVar2;
  int in_A0;
  int *piVar3;
  int unaff_A5;
  int unaff_A6;

  if (in_A0 == 0) {
    *(undefined1 *)(unaff_A6 + 0x6b) = 0xff;
    return;
  }
  bVar1 = *(byte *)(unaff_A6 + 0x6b);
  if (bVar1 == 0xff) {
    piVar3 = (int *)(unaff_A5 + -0x18aa);
    cVar2 = '\0';
    while (*piVar3 != 0) {
      cVar2 = cVar2 + '\x01';
      piVar3 = piVar3 + 2;
      if ((int *)0x1067d5 < piVar3) {
        return;
      }
    }
    *(char *)(unaff_A6 + 0x6b) = cVar2;
  }
  else {
    if (0xf < bVar1) {
      __m68k_trap(0xf);
    }
    piVar3 = (int *)((int)(short)((ushort)bVar1 << 3) + unaff_A5 + -0x18aa);
  }
  *piVar3 = unaff_A6;
  piVar3[1] = in_A0;
  return;
}
