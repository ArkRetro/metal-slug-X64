/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x058c2e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_58c2e(void)

{
  int iVar1;
  undefined4 *unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x20) = 1;
  candidate_59c2c();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    *(undefined4 *)((int)unaff_A6 + 0x82) = 0x5000;
  }
  else {
    *(undefined4 *)((int)unaff_A6 + 0x82) = 0xffffb000;
  }
  candidate_120f8();
  *unaff_A6 = 0x58c66;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar1 = *(int *)((int)unaff_A6 + 0x82) + *(int *)((int)unaff_A6 + 0x62);
    if (-1 < iVar1) {
      iVar1 = 0;
      *(undefined4 *)((int)unaff_A6 + 0x82) = 0;
    }
  }
  else {
    iVar1 = *(int *)((int)unaff_A6 + 0x82) + *(int *)((int)unaff_A6 + 0x62);
    if (iVar1 < 0) {
      iVar1 = 0;
      *(undefined4 *)((int)unaff_A6 + 0x82) = 0;
    }
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar1;
  candidate_59214();
  if ((*(char *)((int)unaff_A6 + 0x8d) != '\0') && (*(int *)((int)unaff_A6 + 0x82) == 0)) {
    *(undefined2 *)((int)unaff_A6 + 0x86) = 1;
    *unaff_A6 = candidate_58cbe;
  }
  candidate_5929a();
  return;
}
