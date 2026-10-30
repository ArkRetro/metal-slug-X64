/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x013f1e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_13f1e(void)

{
  int iVar1;
  byte bVar2;
  int unaff_A5;
  int unaff_A6;

  bVar2 = *(byte *)(unaff_A6 + 0x6a);
  if (bVar2 != 0xff) {
    if (7 < bVar2) {
      __m68k_trap(0xf);
    }
    iVar1 = unaff_A5 + 0x406e + (int)(short)((ushort)bVar2 << 6);
    *(undefined4 *)(iVar1 + 0x26) = 0;
    *(undefined2 *)(iVar1 + 0x2a) = 2;
    *(undefined1 *)(unaff_A6 + 0x6a) = 0xff;
    candidate_1abc8();
  }
  return;
}
