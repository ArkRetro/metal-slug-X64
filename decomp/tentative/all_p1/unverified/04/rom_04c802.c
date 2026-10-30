/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04c802.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4c802(void)

{
  code *pcVar1;
  int iVar2;
  short *psVar3;
  short sVar4;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x86);
  psVar3 = (short *)(unaff_A6[3] + 0x86);
  sVar4 = *psVar3;
  uVar5 = sVar4 == 0;
  *psVar3 = sVar4 + -1;
  candidate_1ac38();
  candidate_427a();
  if ((bool)uVar5) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  if (*(char *)((int)unaff_A6 + 0x9f) == '\0') {
    candidate_49e8();
  }
  candidate_4e07a();
  return;
}
