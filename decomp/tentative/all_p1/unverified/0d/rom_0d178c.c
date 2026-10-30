/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d178c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_d178c(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_4256();
  candidate_1ad6e();
  *unaff_A6 = 0xd17b0;
  candidate_d1b46();
  candidate_d1b28();
  if ((_DAT_0010000e & 0x1f) == 0) {
    uVar1 = candidate_29c6();
    if ((uVar1 & 7) == 0) {
      *unaff_A6 = candidate_d17f6;
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  bVar2 = false;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  candidate_19424();
  if (bVar2) {
    *unaff_A6 = 0xd166a;
  }
  return;
}
