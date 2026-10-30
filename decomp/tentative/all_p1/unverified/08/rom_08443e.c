/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08443e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8443e(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_1ad6e();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x8446c;
  candidate_1abc8();
  iVar2 = unaff_A6[3];
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(iVar2 + 0x1e);
  if (*(char *)(iVar2 + 0x61) != '\0') {
    *unaff_A6 = candidate_844c2;
  }
  uVar3 = 0;
  if ((*(byte *)(iVar2 + 0x15) & 0x40) == 0) {
    uVar3 = 0;
    candidate_4256();
  }
  else {
    candidate_427a();
  }
  candidate_409a();
  if (!(bool)uVar3) {
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
