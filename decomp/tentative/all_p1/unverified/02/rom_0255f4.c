/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0255f4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void thunk_FUN_000256d4(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x9a) = 0xfff0;
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0xfff00000;
  unaff_A6[0x20] = 0xf0000;
  *unaff_A6 = 0x25728;
  uVar3 = 0;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  if ((bool)uVar3) {
    candidate_13f1e();
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  if ((int)unaff_A6[0x20] < 0) {
    candidate_13f1e();
  }
  else {
    candidate_140c0();
    unaff_A6[0xd] = unaff_A6[0x20] + unaff_A6[0xd];
    unaff_A6[0x20] = unaff_A6[0x20] + -0x14000;
  }
  return;
}
