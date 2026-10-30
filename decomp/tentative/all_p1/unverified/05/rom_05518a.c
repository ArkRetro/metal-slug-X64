/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05518a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5518a(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfb;
  candidate_1ad6e();
  candidate_4256();
  *unaff_A6 = 0x551be;
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  if (!(bool)uVar3) {
    return;
  }
  *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
