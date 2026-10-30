/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07d678.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7d678(void)

{
  code *pcVar1;
  int iVar2;
  ushort extraout_D0u;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_1b1dc();
  candidate_4256();
  *(uint *)((int)unaff_A6 + 0x62) = (uint)extraout_D0u << 0x10;
  *unaff_A6 = 0x7d6a8;
  candidate_1456a();
  candidate_427a();
  uVar3 = *(char *)(unaff_A6[3] + 0x6e) != -1;
  if (!(bool)uVar3) {
    *unaff_A6 = candidate_b0626;
  }
  candidate_7d8a6();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_7d704;
    uVar3 = 0;
  }
  candidate_8d972();
  if ((bool)uVar3) {
    *(undefined1 *)(unaff_A6[3] + 0x61) = 0xee;
    uVar3 = 0;
  }
  candidate_7d8e2();
  if ((bool)uVar3) {
    *(undefined1 *)(unaff_A6[3] + 0x61) = 0xee;
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
