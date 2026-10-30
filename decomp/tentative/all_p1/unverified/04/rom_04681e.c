/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04681e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4681e(void)

{
  byte *pbVar1;
  byte bVar2;
  int unaff_A6;

  pbVar1 = *(byte **)(unaff_A6 + 0x86);
  if ((char)*pbVar1 < '\0') {
    return;
  }
  *(ushort *)(unaff_A6 + 0x8a) = (ushort)*pbVar1;
  bVar2 = pbVar1[1];
  *(ushort *)(unaff_A6 + 0x8c) = (ushort)bVar2 * 2;
  *(char *)(unaff_A6 + 0x8e) = -(bVar2 == 0);
  *(undefined1 *)(unaff_A6 + 0x8f) = 0;
  *(byte **)(unaff_A6 + 0x86) = pbVar1 + 2;
  return;
}
