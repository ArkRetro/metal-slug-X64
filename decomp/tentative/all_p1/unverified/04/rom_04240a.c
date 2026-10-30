/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04240a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4240a(void)

{
  code *pcVar1;
  short sVar2;
  int unaff_A6;

  sVar2 = (ushort)*(byte *)(unaff_A6 + 0x80) << 2;
  pcVar1 = *(code **)(*(int *)(sVar2 + 0x44ecc) +
                     (int)(short)(CONCAT11((char)((ushort)sVar2 >> 8),
                                           *(undefined1 *)(unaff_A6 + 0x9f)) * 4));
  if (pcVar1 == (code *)0xffffffff) {
    return;
  }
  (*pcVar1)();
  return;
}
