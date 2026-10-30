/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x085952.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_85952(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_4256();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  candidate_1aca2();
  *(undefined1 *)((int)unaff_A6 + 0x12) = *(undefined1 *)(extraout_A0 + 0x12);
  *(undefined1 *)((int)unaff_A6 + 0x13) = *(undefined1 *)(extraout_A0 + 0x13);
  candidate_1ad6e();
  *(undefined1 *)((int)unaff_A6 + 0x12) = 0xff;
  *(undefined1 *)((int)unaff_A6 + 0x13) = 0xff;
  *unaff_A6 = 0x859cc;
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
