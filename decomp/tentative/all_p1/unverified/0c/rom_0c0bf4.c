/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c0bf4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c0bf4(void)

{
  short *psVar1;
  short sVar2;
  int extraout_A0;
  int unaff_A6;
  bool bVar3;

  bVar3 = false;
  sVar2 = candidate_c0bae();
  if (bVar3) {
    *(byte *)(extraout_A0 + 0x9a) = *(byte *)(extraout_A0 + 0x9a) | 0x10;
    psVar1 = (short *)((short)(sVar2 * 2) + 0x10c728);
    *psVar1 = *psVar1 + 0x100;
    if ((*(char *)(unaff_A6 + 0x93) != '\x02') &&
       (0x77f < *(ushort *)((short)(sVar2 * 2) + 0x10c728))) {
      *(undefined1 *)(extraout_A0 + 0x94) = 2;
    }
  }
  return;
}
