/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06fa76.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_6fa76(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  *(undefined2 *)(unaff_A6 + 0x1d) = *(undefined2 *)(unaff_A6[3] + 0x74);
  if (*(short *)(unaff_A6 + 0x1d) < 0) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  candidate_1ad6e();
  candidate_4256();
  candidate_9ea8();
  *unaff_A6 = 0x6faea;
  bVar3 = false;
  candidate_409a();
  if (bVar3) {
    *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
    uVar4 = 0;
    candidate_1abc8();
    candidate_427a();
    if (!(bool)uVar4) {
      return;
    }
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
