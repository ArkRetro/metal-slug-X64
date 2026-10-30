/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c8abc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c8abc(void)

{
  ushort uVar1;
  ushort uVar2;
  int in_A0;
  int unaff_A5;

  uVar1 = *(ushort *)(in_A0 + 0x14) & 0xfff8;
  *(ushort *)(in_A0 + 0x14) = uVar1;
  uVar2 = *(ushort *)(in_A0 + 0x18) & 0xfff8;
  *(ushort *)(in_A0 + 0x18) = uVar2;
  if (((uVar1 == *(ushort *)(in_A0 + 0x1c)) && (uVar2 == *(ushort *)(in_A0 + 0x20))) &&
     (*(char *)(unaff_A5 + -0x7f72) == '\0')) {
    return;
  }
  return;
}
