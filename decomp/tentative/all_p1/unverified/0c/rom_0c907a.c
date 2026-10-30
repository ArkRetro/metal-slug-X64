/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c907a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c907a(void)

{
  byte bVar1;
  undefined1 uVar2;
  int unaff_A5;

  bVar1 = *(char *)(unaff_A5 + 0x69df) + 1;
  *(byte *)(unaff_A5 + 0x69df) = bVar1;
  if (*(byte *)(unaff_A5 + 0x69de) <= bVar1) {
    *(undefined1 *)(unaff_A5 + 0x69df) = 0;
    bVar1 = *(byte *)(unaff_A5 + 0x69dd);
    *(byte *)(unaff_A5 + 0x69dd) = bVar1 - 8;
    uVar2 = bcdAdjust(*(char *)(unaff_A5 + 0x69dc) - (bVar1 < 8));
    if (bVar1 < 8 && *(char *)(unaff_A5 + 0x69dc) == '\0') {
      uVar2 = 0;
    }
    *(undefined1 *)(unaff_A5 + 0x69dc) = uVar2;
  }
  return;
}
