/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x051772.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_51772(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_1ad6e();
  candidate_9ea8();
  candidate_50fb4();
  candidate_4256();
  *unaff_A6 = 0x517b8;
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar3) {
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
