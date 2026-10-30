/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x034124.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_34124(void)

{
  ushort uVar1;
  ushort uVar2;
  int unaff_A6;

  uVar1 = candidate_29c6();
  uVar2 = candidate_29c6();
  if ((uVar2 & 1) == 0) {
    *(ushort *)(unaff_A6 + 0x32) = uVar1 & 0xfff;
    *(undefined2 *)(unaff_A6 + 0x30) = 0;
  }
  else {
    *(ushort *)(unaff_A6 + 0x32) = -(uVar1 & 0xfff);
    *(undefined2 *)(unaff_A6 + 0x30) = 0xffff;
  }
  return uVar2 & 1;
}
