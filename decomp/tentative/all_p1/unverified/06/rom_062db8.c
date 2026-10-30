/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x062db8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_62db8(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x7ff;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x7ff;
  candidate_9ea8();
  candidate_4520();
  candidate_4256();
  unaff_A6[0x14] = 0xe1294;
  candidate_1ad6e();
  candidate_1b1dc();
  *unaff_A6 = 0x62e0a;
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
  }
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
