/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0436b6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_436b6(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_1ad6e();
  candidate_43792();
  candidate_9ea8();
  candidate_9ea8();
  if (*(char *)(unaff_A6 + 0x20) == '\0') {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 100;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 100;
  }
  else {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 300;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 300;
  }
  *(undefined1 *)(unaff_A6 + 0x22) = 0;
  *unaff_A6 = 0x43712;
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar3) {
    *(undefined1 *)(unaff_A6[3] + 0x60) = 1;
    candidate_4192();
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  if (*(char *)(unaff_A6 + 0x22) != '\0') {
    candidate_43740();
  }
  return;
}
