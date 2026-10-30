/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0886ec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_886ec(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0xaa) = 0;
  candidate_120f8();
  candidate_19dfc();
  *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
  candidate_8869c();
  candidate_13f1e();
  if (*(short *)(unaff_A6 + 0x1d) == 0) {
    unaff_A6[0x28] = 0;
  }
  else if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    unaff_A6[0x28] = 0xe4f8e;
  }
  else {
    unaff_A6[0x28] = 0xe4fc0;
  }
  candidate_4e76();
  *unaff_A6 = 0x88760;
  uVar3 = 0;
  candidate_1436a();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_887b2;
  }
  uVar3 = 0;
  if (unaff_A6[0x28] != 0) {
    uVar3 = 0;
    candidate_140c0();
  }
  candidate_49e8();
  candidate_1945c();
  if (!(bool)uVar3) {
    return;
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
