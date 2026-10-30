/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0877de.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_877de(void)

{
  uint uVar1;
  uint uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  uVar1 = candidate_29c6();
  uVar2 = uVar1 & 0xf;
  if ((uVar1 & 1) != 0) {
    uVar2 = -uVar2;
  }
  unaff_A6[0xc] = uVar2 << 0xc;
  candidate_4256();
  *unaff_A6 = 0x87812;
  uVar3 = 0;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    unaff_A6[0xc] = 0;
    uVar3 = 0;
  }
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_41ac;
    uVar3 = 0;
  }
  candidate_19504();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
