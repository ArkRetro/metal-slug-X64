/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08d188.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_8d188(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  unaff_A6[0xd] = 0;
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0;
  *unaff_A6 = 0x8d196;
  unaff_A6[0xd] = unaff_A6[0xd] + 0x3000;
  sVar3 = *(short *)((int)unaff_A6 + 0x6e);
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + 1;
                    /* WARNING (jumptable): Sanity check requires truncation of jumptable */
                    /* WARNING: Could not find normalized switch variable to match jumptable */
  switch(*(undefined1 *)(sVar3 + 0xe55ca)) {
  case 0:
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    candidate_1ac38();
    candidate_427a();
    return;
  case 1:
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    candidate_1ac38();
    candidate_427a();
    return;
  case 2:
    candidate_1ac38();
    return;
  case 3:
  case 4:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 5:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  default:
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
  }
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
