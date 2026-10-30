/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x019e40.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_19e40(void)

{
  char *pcVar1;
  int iVar2;
  int unaff_A5;
  int unaff_A6;

  *(undefined4 *)(unaff_A5 + 0x4326) = 0;
  *(undefined4 *)(unaff_A5 + 0x432a) = 0;
  *(undefined1 *)(unaff_A5 + 0x432e) = 0;
  for (iVar2 = *(int *)(unaff_A6 + 8); unaff_A6 != iVar2; iVar2 = *(int *)(iVar2 + 8)) {
    if (*(byte *)(iVar2 + 0x69) < 9) {
      pcVar1 = (char *)(unaff_A5 + 0x4326 + (int)(short)(ushort)*(byte *)(iVar2 + 0x69));
      *pcVar1 = *pcVar1 + '\x01';
    }
  }
  return;
}
