/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "ref.h"
#include "node3d.h"
#include "base/color.h"

namespace rolling{
    class Line3D : public Node3D{
    public:
        using thisType = Line3D;
        template <typename... Args>
        static base::SharedPtr<thisType> create(Args&&... args) {
            return base::SharedPtr<thisType>(new thisType(std::forward<Args>(args)...));
        }

    public:
        Line3D();
        virtual ~Line3D();

        Line3D(const Line3D&) = delete;
        Line3D(Line3D&&) = delete;
        Line3D& operator=(const Line3D&) = delete;
        Line3D& operator=(Line3D&&) = delete;

		void Update(rolling::Matrix4x4& modelViewTransform) override;
		void Render(rolling::Matrix4x4& modelViewTransform) override;

		void SetPos1(const rolling::VectorF3& pos);
		void SetPos2(const rolling::VectorF3& pos);

		void SetColor(const Color& color);
		void SetColor1(const Color& color);
		void SetColor2(const Color& color);

	private:
		rolling::VectorF4 pos1;
		rolling::VectorF4 pos2;

		Color color1 = Color::WHITE;
		Color color2 = Color::WHITE;
    };
};