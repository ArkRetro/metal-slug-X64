/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06243e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6243e(void)

{
  ushort uVar1;
  short sVar3;
  int iVar2;
  int unaff_A6;

  uVar1 = (ushort)*(byte *)(unaff_A6 + 0x97);
  if ((char)*(byte *)(unaff_A6 + 0x97) < '\0') {
    uVar1 = candidate_29c6();
  }
  sVar3 = (uVar1 & 3) * 0xc;
  *(undefined4 *)(unaff_A6 + 0x34) = *(undefined4 *)(sVar3 + 0x2d4ccc);
  *(undefined4 *)(unaff_A6 + 0x7a) = *(undefined4 *)(sVar3 + 0x2d4cd0);
  iVar2 = *(int *)(sVar3 + 0x2d4cd4);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    iVar2 = -iVar2;
  }
  *(int *)(unaff_A6 + 0x30) = iVar2;
  return;
}
