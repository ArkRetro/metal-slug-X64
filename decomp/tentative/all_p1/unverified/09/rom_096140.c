/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x096140.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_96140(void)

{
  code *pcVar1;
  int iVar2;
  undefined2 uVar3;
  char extraout_D1b;
  char unaff_D3b;
  int unaff_A5;
  undefined4 *unaff_A6;

  if (*(short *)(unaff_A6 + 0x1c) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  candidate_1ad6e();
  uVar3 = *(undefined2 *)(unaff_A6[3] + 0x86);
  *(undefined2 *)((int)unaff_A6 + 0x86) = uVar3;
  *(char *)(unaff_A6 + 0x24) = (char)uVar3;
  candidate_9ea8();
  candidate_9ea8();
  func_0x00009cce();
  *(undefined2 *)(unaff_A6 + 0x21) = *(undefined2 *)(unaff_A6[3] + 0x84);
  *(undefined1 *)((int)unaff_A6 + 0x91) = 99;
  *unaff_A6 = 0x961ce;
  if (*(char *)(unaff_A6[3] + 0x94) != '\0') {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    candidate_c05ec();
    if ((extraout_D1b != '\0') && (unaff_D3b != '\0')) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    }
    if (*(byte *)(unaff_A6[3] + 0x91) != *(byte *)((int)unaff_A6 + 0x91)) {
      *(byte *)((int)unaff_A6 + 0x91) = *(byte *)(unaff_A6[3] + 0x91) & 0xf;
      candidate_4256();
    }
    *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
    candidate_1abc8();
    candidate_427a();
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
