#!/usr/bin/env python3
"""
OpenRouter API를 통해 여러 AI 모델의 의견을 수집하는 스크립트
"""

import os
import json
from openai import OpenAI

# OpenRouter API 클라이언트 설정
client = OpenAI(
    api_key=os.environ.get("OPENROUTER_API_KEY"),
    base_url="https://openrouter.ai/api/v1"
)

# 질문할 프롬프트
PROMPT = """
C++과 LuaJIT을 연동하여 다음 기능을 구현하려고 합니다:

1. UDP 소켓을 사용하여 데이터를 전송하는 기능
2. 콘솔에 로그를 출력하는 기능 (INFO, WARNING, ERROR 레벨)

이 두 기능을 C++로 구현하고, LuaJIT의 FFI를 사용하여 Lua 스크립트에서 호출할 수 있도록 만들려고 합니다.
Visual Studio 환경에서 DLL로 빌드할 예정입니다.

다음 사항에 대해 구체적인 조언을 부탁드립니다:

1. C++ 함수의 시그니처 설계 (extern "C" 사용, 메모리 관리 등)
2. UDP 소켓 구현 시 주의사항 (Windows 환경)
3. LuaJIT FFI 바인딩 시 고려사항
4. 에러 처리 전략
5. 스레드 안전성 고려사항

간결하고 실용적인 조언을 부탁드립니다.
"""

# 테스트할 모델 목록
MODELS = [
    "anthropic/claude-3.5-sonnet",
    "openai/gpt-4-turbo",
    "google/gemini-pro-1.5"
]

def consult_model(model_name: str, prompt: str) -> dict:
    """특정 모델에게 질문하고 응답을 받습니다."""
    print(f"\n{'='*80}")
    print(f"Consulting: {model_name}")
    print(f"{'='*80}")
    
    try:
        response = client.chat.completions.create(
            model=model_name,
            messages=[
                {"role": "user", "content": prompt}
            ],
            max_tokens=2000,
            temperature=0.7
        )
        
        answer = response.choices[0].message.content
        print(f"\nResponse received ({len(answer)} characters)")
        
        return {
            "model": model_name,
            "success": True,
            "response": answer,
            "tokens": response.usage.total_tokens if hasattr(response, 'usage') else None
        }
    
    except Exception as e:
        print(f"\nError: {str(e)}")
        return {
            "model": model_name,
            "success": False,
            "error": str(e)
        }

def main():
    """메인 함수"""
    print("Starting AI consultation process...")
    print(f"Models to consult: {', '.join(MODELS)}")
    
    results = []
    
    for model in MODELS:
        result = consult_model(model, PROMPT)
        results.append(result)
    
    # 결과를 JSON 파일로 저장
    output_file = "/home/ubuntu/cpp-luajit-udp-logger/docs/ai_consultation_results.json"
    with open(output_file, 'w', encoding='utf-8') as f:
        json.dump(results, f, ensure_ascii=False, indent=2)
    
    print(f"\n{'='*80}")
    print(f"Results saved to: {output_file}")
    print(f"{'='*80}")
    
    # 성공한 응답들을 마크다운으로 정리
    md_output = "/home/ubuntu/cpp-luajit-udp-logger/docs/ai_consultation_summary.md"
    with open(md_output, 'w', encoding='utf-8') as f:
        f.write("# AI 모델 컨설팅 결과\n\n")
        f.write("## 질문\n\n")
        f.write(PROMPT + "\n\n")
        f.write("---\n\n")
        
        for result in results:
            if result["success"]:
                f.write(f"## {result['model']}\n\n")
                f.write(result['response'] + "\n\n")
                f.write("---\n\n")
            else:
                f.write(f"## {result['model']} (실패)\n\n")
                f.write(f"Error: {result['error']}\n\n")
                f.write("---\n\n")
    
    print(f"Summary saved to: {md_output}")
    print("\nConsultation complete!")

if __name__ == "__main__":
    main()

