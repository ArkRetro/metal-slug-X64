/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x034f1a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_34f1a(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_1ad6e();
  *(undefined4 *)((int)unaff_A6 + 0x72) =
       *(undefined4 *)((short)(*(short *)(unaff_A6 + 0x1c) * 4) + 0x23a58e);
  candidate_9ea8();
  candidate_9ea8();
  func_0x00009cce();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  candidate_350e4();
  *unaff_A6 = 0x34f96;
  candidate_1abc8();
  iVar2 = unaff_A6[3];
  if (*(char *)(iVar2 + 0x89) != '\0') {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(iVar2 + 0x1e);
  *(undefined2 *)((int)unaff_A6 + 0x7e) = *(undefined2 *)(iVar2 + 0x7e);
  if (*(short *)(iVar2 + 0x76) != *(short *)((int)unaff_A6 + 0x76)) {
    candidate_350e4();
  }
  candidate_427a();
  return;
}
