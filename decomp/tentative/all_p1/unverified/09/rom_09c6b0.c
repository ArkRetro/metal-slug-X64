/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09c6b0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9c6b0(void)

{
  code *pcVar1;
  int iVar2;
  int *piVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x24) & 0x40) == 0) {
    candidate_9ea8();
  }
  else {
    candidate_9ea8();
  }
  if ((*(byte *)(unaff_A6 + 0x24) & 8) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  candidate_1ad6e();
  *unaff_A6 = 0x9c718;
  piVar3 = (int *)unaff_A6[3];
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)((int)piVar3 + 0x1e);
  if ((*piVar3 != 0x9b6e6) && (*piVar3 != 0x9bc30)) {
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
  candidate_1abc8();
  candidate_427a();
  return;
}
